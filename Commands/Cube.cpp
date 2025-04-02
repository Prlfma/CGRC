#include "Cube.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

int Cube::execute(const std::map<std::string, std::string>& args)
{
    if (args.find("L") == args.end() || args.find("origin") == args.end() || args.find("filepath") == args.end())
    {
        std::cerr << "Error: Missing one or more arguments.\n";
        return 3;
    }

    double L = std::stod(args.at("L"));
    if (L <= 0)
    {
        std::cerr << "Error: Cube side length must be positive.\n";
        return 1;
    }

    Vec origin;
    std::stringstream ss(args.at("origin"));
    char ignore;
    if (!(ss >> ignore >> origin.x >> ignore >> origin.y >> ignore >> origin.z >> ignore))
    {
        std::cerr << "Error: Invalid origin format.\n";
        return 3;
    }

    std::string filepath = args.at("filepath");
    if (filepath.empty())
    {
        std::cerr << "Error: Invalid file path.\n";
        return 2;
    }

    TriangleSoup triangles = generateCube(L, origin);

    STLParser parser;
    parser.write(triangles, filepath);

    std::cout << "Cube successfully saved to " << filepath << "\n";
    return 0;
}
const std::string& Cube::getName() const {
    static const std::string name = "cube";
    return name;
}

TriangleSoup Cube::generateCube(double L, const Vec& origin)
{
    TriangleSoup triangles;
    double half = L / 2.0;

    Vec v[8] = {
        {origin.x - half, origin.y - half, origin.z - half},
        {origin.x + half, origin.y - half, origin.z - half},
        {origin.x + half, origin.y + half, origin.z - half},
        {origin.x - half, origin.y + half, origin.z - half},
        {origin.x - half, origin.y - half, origin.z + half},
        {origin.x + half, origin.y - half, origin.z + half},
        {origin.x + half, origin.y + half, origin.z + half},
        {origin.x - half, origin.y + half, origin.z + half},
    };

    
    int faces[6][4] = {
        {0, 1, 2, 3}, {4, 5, 6, 7}, 
        {0, 1, 5, 4}, {2, 3, 7, 6}, 
        {0, 3, 7, 4}, {1, 2, 6, 5}  
    };

    for (auto& face : faces)
    {
        STLParser parser;
        Vec normal = parser.calculateNormal(v[face[0]], v[face[1]], v[face[2]]);
        triangles.push_back({v[face[0]], normal});
        triangles.push_back({v[face[1]], normal});
        triangles.push_back({v[face[2]], normal});

        triangles.push_back({v[face[2]], normal});
        triangles.push_back({v[face[3]], normal});
        triangles.push_back({v[face[0]], normal});
    }

    return triangles;
}
