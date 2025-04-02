#pragma once
#include "Command.h"
#include "../Parser/STLParser.h"

class Cube : public Command
{
    public:
    const std::string& getName() const override;
    int execute(const std::map<std::string, std::string>& args) override;
    private:
    TriangleSoup generateCube(double L, const Vec& origin);
};