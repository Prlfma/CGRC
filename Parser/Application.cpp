#include "Application.h"
#include <iostream>
#include <sstream>
#include <map>

void Application::registerCommand(std::unique_ptr<Command> command)
{
    const std::string& name = command->getName();
    commands.emplace(name, std::move(command));
}

int Application::execute(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "Error: No command specified.\n";
        return 1;
    }

    std::string commandName = argv[1];
    auto cmdIt = commands.find(commandName);
    if (cmdIt == commands.end()) {
        std::cerr << "Error: Unknown command '" << commandName << "'.\n";
        return 1;
    }

    std::map<std::string, std::string> args;
    for (int i = 2; i < argc; i += 2) {
        if (i + 1 >= argc) break;
        std::string key = argv[i];
        if (key.find("--") == 0) {
            key = key.substr(2);
        }
        args[key] = argv[i + 1];
    }

    return cmdIt->second->execute(args);
}