#include "Sphere.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <math.h>

constexpr double M_PI = 3.14159265358979323846;
const int LATITUDE_DIVS = 20;  // Поділ за широтою
const int LONGITUDE_DIVS = 20; // Поділ за довготою

const std::string& Sphere::getName() const {
    static const std::string name = "sphere";
    return name;
}

int Sphere::execute(const std::map<std::string, std::string>& args)
{
    if (args.find("R") == args.end() || args.find("origin") == args.end() || args.find("filepath") == args.end())
    {
        std::cerr << "Error: Missing one or more arguments.\n";
        return 3;
    }

    double R = std::stod(args.at("R"));
    if (R <= 0)
    {
        std::cerr << "Error: Sphere radius must be positive.\n";
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

    TriangleSoup triangles = generateSphere(R, origin);

    STLParser parser;
    parser.write(triangles, filepath);

    std::cout << "Sphere successfully saved to " << filepath << "\n";
    return 0;
}

TriangleSoup Sphere::generateSphere(double R, const Vec& origin)
{
    TriangleSoup triangles;
    std::vector<std::vector<Vec>> vertices(LATITUDE_DIVS + 1, std::vector<Vec>(LONGITUDE_DIVS + 1));

    
    for (int i = 0; i <= LATITUDE_DIVS; ++i)
    {
        double theta = M_PI * i / LATITUDE_DIVS; 
        for (int j = 0; j <= LONGITUDE_DIVS; ++j)
        {
            double phi = 2 * M_PI * j / LONGITUDE_DIVS; 
            double x = R * sin(theta) * cos(phi) + origin.x;
            double y = R * sin(theta) * sin(phi) + origin.y;
            double z = R * cos(theta) + origin.z;
            vertices[i][j] = {x, y, z};
        }
    }

    for (int i = 0; i < LATITUDE_DIVS; ++i)
    {
        for (int j = 0; j < LONGITUDE_DIVS; ++j)
        {
            Vec v1 = vertices[i][j];
            Vec v2 = vertices[i + 1][j];
            Vec v3 = vertices[i][j + 1];
            Vec v4 = vertices[i + 1][j + 1];

            STLParser parser;
            Vec normal1 = parser.calculateNormal(v1, v2, v3);
            triangles.push_back({v1, normal1});
            triangles.push_back({v2, normal1});
            triangles.push_back({v3, normal1});

            Vec normal2 = parser.calculateNormal(v3, v2, v4);
            triangles.push_back({v3, normal2});
            triangles.push_back({v2, normal2});
            triangles.push_back({v4, normal2});
        }
    }

    return triangles;
}
