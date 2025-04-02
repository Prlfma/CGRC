#pragma once
#include "Commands/Command.h"
#include <memory>
#include <unordered_map>
#include <string>

class Application
{
public:
    void registerCommand(std::unique_ptr<Command> command);
    int execute(int argc, char *argv[]);
    
private:
    std::unordered_map<std::string, std::unique_ptr<Command>> commands;
};