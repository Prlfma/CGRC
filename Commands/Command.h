#pragma once
#include <map>
#include <string>

class Command {
public:
    virtual ~Command() = default;
    virtual const std::string& getName() const = 0;
    virtual int execute(const std::map<std::string, std::string>& args) = 0;
};