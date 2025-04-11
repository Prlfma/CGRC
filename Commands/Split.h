#pragma once

#include "Command.h"
#include "../Parser/STLParser.h"
#include <map>
#include <string>

class Split : public Command
{
public:
    const std::string& getName() const override {
        static std::string name = "Split";
        return name;
    }

    int execute(const std::map<std::string, std::string>& args) override;

private:
    bool isAbovePlane(const Vec& point, const Vec& origin, const Vec& normal);
    void splitMesh(const TriangleSoup& inputMesh, const Vec& origin, const Vec& normal, 
                   TriangleSoup& meshAbove, TriangleSoup& meshBelow);
    STLVertex intersectEdgeWithPlane(const STLVertex &v1, const STLVertex &v2,
                    const Vec &planeOrigin, const Vec &planeNormal);
};


