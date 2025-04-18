#include "Terminal.h"
#include "raylib.h"
#include <iostream>
using namespace std;

Terminal::Terminal() {}

void Terminal::Toggle() {
    isOpen = !isOpen;
    input.clear();
}

void Terminal::Update() {
    if (!isOpen) return;

    if (IsKeyPressed(KEY_ENTER)) {
        history.push_back("> " + input);
        ExecuteCommand(input);
        input.clear();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !input.empty()) {
        input.pop_back();
    }
    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 126) {
            input += (char)key;
        }
        key = GetCharPressed();
    }
}

void Terminal::Draw() {
    if (!isOpen) return;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.85f));
    int y = 50;
    for (const auto& line : history) {
        DrawText(line.c_str(), 50, y, 20, GREEN);
        y += 25;
    }
    DrawText((("> " + input).c_str()), 50, y, 20, GREEN);
}


// Commands 


void Terminal::AddCommand(const std::string& name, std::function<void(const std::string&)> func) {
    commandMap[name] = func;
}
void Terminal::InitDefaultCommands() {
    AddCommand("echo", [&](const std::string& args) {
        history.push_back(args);
        
    });

    AddCommand("clear", [&](const std::string&) {
        history.clear();
    });

    AddCommand("cat", [&](const std::string& args) {
        if (fileSystem.find(args) != fileSystem.end()) {
            history.push_back(fileSystem[args]);
        } else {
            history.push_back("No such file: " + args);
        }
    });
    AddCommand("help", [&](const std::string&) {
        std::string cmds;
        for (const auto& cmd : activeCommands)
            cmds += cmd + " ";
        history.push_back("Available: " + cmds);
        
    });
    AddCommand("ls", [&](const std::string&) {
        for (const auto& f : fileSystem) {
            history.push_back(f.first);
        }
    });
}


void Terminal::EnableCommand(const std::string& name) {
    if (commandMap.find(name) != commandMap.end()) {
        if (std::find(activeCommands.begin(), activeCommands.end(), name) == activeCommands.end()) {
            activeCommands.push_back(name);
        }
    } else {
        history.push_back("Cannot enable unknown command: " + name);
    }
}


void Terminal::ExecuteCommand(const std::string& input) {
    
    size_t spacePos = input.find(' ');
    std::string command = input.substr(0, spacePos);
    std::string args = (spacePos != std::string::npos) ? input.substr(spacePos + 1) : "";

    if (std::find(activeCommands.begin(), activeCommands.end(), command) == activeCommands.end()) {
        history.push_back("Command not available in this room: " + command);
        return;
    }

    if (commandMap.find(command) != commandMap.end()) {
        commandMap[command](args);
        
    } else {
        history.push_back("Unknown command: " + command);
    }
}

void Terminal::ClearActiveCommands() {
    activeCommands.clear();
}




// files

void Terminal::AddFile(const std::string& path, const std::string& content) {
    fileSystem[path] = content;
}

void Terminal::ClearFiles() {
    fileSystem.clear();
}
