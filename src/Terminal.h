#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include "raylib.h"
#include <functional>

class Terminal {
public:
    bool isOpen = false;

    Terminal();

    void Toggle();
    void Update();
    void Draw();

    void AddFile(const std::string& path, const std::string& content, const std::string& permission);
    void ClearFiles();

    Sound s_hint;
    Sound s_error;
    std::string input;
    std::vector<std::string> history;
    std::map<std::string, std::pair<std::string, std::string>> fileSystem;
    std::unordered_map<std::string, std::function<void(const std::string&)>> commandMap;
    std::vector<std::string> activeCommands;
    bool inNanoMode = false;
    std::string currentEditingFile;
    std::vector<std::string> nanoBuffer;


    void EnableCommand(const std::string& name);
    void ExecuteCommand(const std::string& cmd);
    void AddCommand(const std::string& name, std::function<void(const std::string& args)> handler);
    void ClearActiveCommands();
    void InitDefaultCommands(); 
};

#endif
