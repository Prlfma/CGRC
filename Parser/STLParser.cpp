#include "STLParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>


Vec STLParser::calculateNormal(const Vec& v1, const Vec& v2, const Vec& v3)
{
    Vec u = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
    Vec v = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};
    
    Vec normal = {
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x
    };
    
    double length = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    if (length > 0.0)
    {
        normal.x /= length;
        normal.y /= length;
        normal.z /= length;
    }
    return normal;
}

TriangleSoup STLParser::read(const std::string& filename)
{
    TriangleSoup triangles;
    std::ifstream file("Assets/" + filename);
    if (!file)
    {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return triangles;
    }

    std::string line;
    Vec normal;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string word;
        iss >> word;

        if (word == "facet")
        {
            iss >> word; 
            iss >> normal.x >> normal.y >> normal.z;
        }
        else if (word == "vertex")
        {
            Vertex v;
            v.normal = normal;
            iss >> v.pos.x >> v.pos.y >> v.pos.z;
            triangles.push_back(v);
        }
    }
    return triangles;
}

void STLParser::write(const TriangleSoup& triangleSoup, const std::string& filename)
{
    std::ofstream file("Assets/" + filename);
    if (!file)
    {
        std::cerr << "Error: Cannot write to file " << filename << std::endl;
        return;
    }

    file << "solid STLModel\n";
    for (size_t i = 0; i < triangleSoup.size(); i += 3)
    {
        Vec normal = calculateNormal(triangleSoup[i].pos, triangleSoup[i+1].pos, triangleSoup[i+2].pos);
        file << "  facet normal " << normal.x << " " << normal.y << " " << normal.z << "\n";
        file << "    outer loop\n";
        for (size_t j = 0; j < 3; ++j)
        {
            file << "      vertex " << triangleSoup[i + j].pos.x << " " << triangleSoup[i + j].pos.y << " " << triangleSoup[i + j].pos.z << "\n";
        }
        file << "    endloop\n  endfacet\n";
    }
    file << "endsolid STLModel\n";
}
