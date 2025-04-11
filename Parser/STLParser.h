#pragma once
#include <vector>
#include <string>

struct Vec {double x,y,z;
    Vec operator-(const Vec& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }
    
    // Оператор додавання векторів
    Vec operator+(const Vec& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }
    
    // Множення вектора на скаляр
    Vec operator*(double scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }
};

struct STLVertex
{
    Vec pos;
    Vec normal;
};

using TriangleSoup = std::vector<STLVertex>;

class STLParser
{
    public:
    TriangleSoup read(const std::string& filename);
    void write(const TriangleSoup& triangleSoup, const std::string& filname);
    Vec calculateNormal(const Vec& v1, const Vec& v2, const Vec& v3);
    private:
    
};