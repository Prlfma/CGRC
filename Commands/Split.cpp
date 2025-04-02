#include "Split.h"
#include <iostream>
#include <sstream>
#include <cmath>

int Split::execute(const std::map<std::string, std::string> &args)
{
    if (args.find("input") == args.end() || args.find("origin") == args.end() ||
        args.find("direction") == args.end() || args.find("output1") == args.end() || args.find("output2") == args.end())
    {
        std::cerr << "Error: Missing one or more arguments.\n";
        return 3;
    }

    
    STLParser parser;
    TriangleSoup inputMesh = parser.read(args.at("input"));
    if (inputMesh.empty())
    {
        std::cerr << "Error: Unable to read input STL file.\n";
        return 2;
    }

    
    Vec origin, direction;
    std::stringstream ssOrigin(args.at("origin")), ssDirection(args.at("direction"));
    char ignore;

    if (!(ssOrigin >> ignore >> origin.x >> ignore >> origin.y >> ignore >> origin.z >> ignore) ||
        !(ssDirection >> ignore >> direction.x >> ignore >> direction.y >> ignore >> direction.z >> ignore))
    {
        std::cerr << "Error: Invalid origin or direction format.\n";
        return 3;
    }

    double normalLength = std::sqrt(direction.x * direction.x +
                                    direction.y * direction.y +
                                    direction.z * direction.z);
    if (normalLength <= 0.0)
    {
        std::cerr << "Error: Normal vector length must be greater than zero.\n";
        return 1;
    }

    direction.x /= normalLength;
    direction.y /= normalLength;
    direction.z /= normalLength;

    TriangleSoup meshAbove, meshBelow;
    splitMesh(inputMesh, origin, direction, meshAbove, meshBelow);

    if (meshAbove.empty() || meshBelow.empty())
    {
        std::cerr << "Error: The plane does not split the mesh.\n";
        return 4;
    }

    parser.write(meshAbove, args.at("output1"));
    parser.write(meshBelow, args.at("output2"));

    std::cout << "Mesh successfully split and saved.\n";
    return 0;
}

bool Split::isAbovePlane(const Vec &point, const Vec &origin, const Vec &normal)
{
    double d = (point.x - origin.x) * normal.x +
               (point.y - origin.y) * normal.y +
               (point.z - origin.z) * normal.z;
    return d > 0.0;
}

Vertex Split::intersectEdgeWithPlane(const Vertex &v1, const Vertex &v2,
                                     const Vec &planeOrigin, const Vec &planeNormal)
{
    Vec edge = v2.pos - v1.pos;
    double dot = planeNormal.x * edge.x + planeNormal.y * edge.y + planeNormal.z * edge.z;

    if (fabs(dot) < 1e-10)
    {
        return v1;
    }

    Vec w = v1.pos - planeOrigin;
    double fac = -(planeNormal.x * w.x + planeNormal.y * w.y + planeNormal.z * w.z) / dot;
    edge.x = edge.x * fac;
    edge.y = edge.y * fac;
    edge.z = edge.z * fac;

    Vertex result;
    result.pos.x = v1.pos.x + edge.x;
    result.pos.y = v1.pos.y + edge.y;
    result.pos.z = v1.pos.z + edge.z;

    result.normal.x = v1.normal.x + fac * (v2.normal.x - v1.normal.x);
    result.normal.y = v1.normal.y + fac * (v2.normal.y - v1.normal.y);
    result.normal.z = v1.normal.z + fac * (v2.normal.z - v1.normal.z);

    return result;
}
void Split::splitMesh(const TriangleSoup &inputMesh, const Vec &origin, const Vec &normal,
                      TriangleSoup &meshAbove, TriangleSoup &meshBelow)
{
    for (size_t i = 0; i < inputMesh.size(); i += 3)
    {
        Vertex v1 = inputMesh[i];
        Vertex v2 = inputMesh[i + 1];
        Vertex v3 = inputMesh[i + 2];

        bool above1 = isAbovePlane(v1.pos, origin, normal);
        bool above2 = isAbovePlane(v2.pos, origin, normal);
        bool above3 = isAbovePlane(v3.pos, origin, normal);

        if (above1 && above2 && above3)
        {
            meshAbove.push_back(v1);
            meshAbove.push_back(v2);
            meshAbove.push_back(v3);
        }
        else if (!above1 && !above2 && !above3)
        {
            meshBelow.push_back(v1);
            meshBelow.push_back(v2);
            meshBelow.push_back(v3);
        }
        else
        {
            std::vector<Vertex> vertices = {v1, v2, v3};
            std::vector<bool> above = {above1, above2, above3};

            std::vector<size_t> aboveIndices, belowIndices;
            for (size_t j = 0; j < 3; ++j)
            {
                if (above[j])
                    aboveIndices.push_back(j);
                else
                    belowIndices.push_back(j);
            }

            if (aboveIndices.size() == 1)
            {
                Vertex a = vertices[aboveIndices[0]];
                Vertex b = vertices[belowIndices[0]];
                Vertex c = vertices[belowIndices[1]];

                Vertex i1 = intersectEdgeWithPlane(a, b, origin, normal);
                Vertex i2 = intersectEdgeWithPlane(a, c, origin, normal);

                meshAbove.push_back(a);
                meshAbove.push_back(i1);
                meshAbove.push_back(i2);

                meshBelow.push_back(b);
                meshBelow.push_back(c);
                meshBelow.push_back(i1);

                meshBelow.push_back(c);
                meshBelow.push_back(i2);
                meshBelow.push_back(i1);
            }
            else
            {
                Vertex a = vertices[belowIndices[0]];
                Vertex b = vertices[aboveIndices[0]];
                Vertex c = vertices[aboveIndices[1]];

                Vertex i1 = intersectEdgeWithPlane(a, b, origin, normal);
                Vertex i2 = intersectEdgeWithPlane(a, c, origin, normal);

                meshBelow.push_back(a);
                meshBelow.push_back(i1);
                meshBelow.push_back(i2);

                meshAbove.push_back(b);
                meshAbove.push_back(c);
                meshAbove.push_back(i1);

                meshAbove.push_back(c);
                meshAbove.push_back(i2);
                meshAbove.push_back(i1);
            }
        }
    }
}

