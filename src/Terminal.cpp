#include "Terminal.h"
#include "raylib.h"
#include <sstream>
#include <iostream>

using namespace std;

Terminal::Terminal() {
    s_hint = LoadSound("../sounds/HintReveal.mp3");
    s_error = LoadSound("../sounds/error.wav");
}

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
    if(inNanoMode){
        DrawText((("# " + input).c_str()), 50, y, 20, GREEN);    
    }else{
        DrawText((("> " + input).c_str()), 50, y, 20, GREEN);
    }
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
            if (fileSystem[args].second.find('r') == std::string::npos) {
                history.push_back("Permission denied: " + args);
                PlaySound(s_error);
            } else {
                std::string content = fileSystem[args].first;
                size_t start = 0;
                size_t end = content.find('\n');
                if(args.substr(0,4)=="hint"){
                    PlaySound(s_hint);
                }
                if (end == std::string::npos) {
                    history.push_back(content);
                } else {
                    while (end != std::string::npos) {
                        std::string line = content.substr(start, end - start);
                        history.push_back(line);
                        start = end + 1;
                        end = content.find('\n', start);
                    }
                    if (start < content.length()) {
                        history.push_back(content.substr(start));
                    }
                }
            }
        } else {
            history.push_back("No such file: " + args);
            PlaySound(s_error);
        }
    });

    AddCommand("chmod", [&](const std::string& args) {
        size_t spacePos = args.find(' ');
        if (spacePos == std::string::npos) {
            history.push_back("Usage: chmod +<permissions> <filename> OR chmod -<permissions> <filename>");
            return;
        }
    
        std::string mode = args.substr(0, spacePos);  // e.g., "+x", "-rw"
        std::string filename = args.substr(spacePos + 1);
    
        if (fileSystem.find(filename) == fileSystem.end()) {
            history.push_back("No such file: " + filename);
            PlaySound(s_error);
            return;
        }
    
        std::string& currentPerms = fileSystem[filename].second;
    
        // Ensure permissions are in lowercase and sorted (e.g., "rwx")
        std::sort(currentPerms.begin(), currentPerms.end());
    
        // Process each permission in the mode (e.g., "+x" or "-rw")
        for (size_t i = 1; i < mode.size(); ++i) {  // Skip '+' or '-'
            char op = mode[0];  // '+' or '-'
            char perm = mode[i];  // 'r', 'w', or 'x'
    
            if (perm != 'r' && perm != 'w' && perm != 'x') {
                history.push_back("Invalid permission: " + std::string(1, perm));
                continue;
            }
    
            if (op == '+') {
                // Add permission if not already present
                if (currentPerms.find(perm) == std::string::npos) {
                    currentPerms += perm;
                    std::sort(currentPerms.begin(), currentPerms.end());  // Keep sorted
                }
            } else if (op == '-') {
                // Remove permission if present
                size_t pos = currentPerms.find(perm);
                if (pos != std::string::npos) {
                    currentPerms.erase(pos, 1);
                }
            } else {
                history.push_back("Invalid operator: " + std::string(1, op));
            }
        }
    
        history.push_back("Permissions of " + filename + " updated to " + currentPerms);
    });

    AddCommand("help", [&](const std::string&) {
        std::string cmds;
        for (const auto& cmd : activeCommands)
            cmds += cmd + " ";
        history.push_back("Available: " + cmds);
    });

    AddCommand("ls", [&](const std::string& args) {
        if(args == "-l"){
            for (const auto& f : fileSystem) {
                history.push_back(f.first + " " + fileSystem[f.first].second);
            }
        }else{
            for (const auto& f : fileSystem) {
                history.push_back(f.first);
            }
        } 
    });

    AddCommand("rm", [&](const std::string& args) {
        if (fileSystem.find(args) != fileSystem.end()) {
            if(fileSystem[args].second.find('w') == std::string::npos){
                history.push_back("Permission denied no write permission");
                PlaySound(s_error);
            }else{
                fileSystem.erase(args);
                history.push_back("File deleted: " + args);
            }
        } else {
            history.push_back("No such file: " + args);
            PlaySound(s_error);
        }
    });
    
    AddCommand("touch", [&](const std::string& args) {
        if (args.empty()) {
            history.push_back("Usage: touch <filename>");
            return;
        }
    
        if (fileSystem.find(args) == fileSystem.end()) {
            fileSystem[args] = { "", "rw" };
            history.push_back("File created: " + args);
        } else {
            history.push_back("File already exists: " + args);
        }
    });

    AddCommand("man", [&](const std::string& args) {
        if (args == "ls") {
            history.push_back("NAME");
            history.push_back("    ls - List all files");
            history.push_back(" ");
            history.push_back("USAGE");
            history.push_back("    ls");
            history.push_back("    ls -l : to show permissions");
        } 
        else if (args == "cat") {
            history.push_back("NAME");
            history.push_back("    cat - Display the contents of a file");
            history.push_back(" ");
            history.push_back("USAGE");
            history.push_back("    cat <filename>");
        } 
        else if (args == "echo") {
            history.push_back("NAME");
            history.push_back("    echo - Print text to terminal");
            history.push_back(" ");
            history.push_back("USAGE");
            history.push_back("    echo <message>");
        } 
        else if (args == "clear") {
            history.push_back("NAME");
            history.push_back("    clear - Clear the terminal history");
        } 
        else if (args == "help") {
            history.push_back("NAME");
            history.push_back("    help - List all active commands");
        } 
        else if (args == "chmod") {
            history.push_back("NAME");
            history.push_back("    chmod - Change file permissions");
            history.push_back(" ");
            history.push_back("USAGE");
            history.push_back("    chmod <permissions> <filename>");
        } 
        else if (args == "rm") {
            history.push_back("NAME");
            history.push_back("    rm - Remove a file");
            history.push_back(" ");
            history.push_back("USAGE");
            history.push_back("    rm <filename>");
        } 
        else if (args == "touch") {
            history.push_back("NAME");
            history.push_back("    touch - Create a new empty file");
            history.push_back(" ");
            history.push_back("USAGE");
            history.push_back("    touch <filename>");
        } 
        else if (args == "man") {
            history.push_back("NAME");
            history.push_back("    man - Show the manual of a command");
            history.push_back(" ");
            history.push_back("USAGE");
            history.push_back("    man <command>");
        } 
        else {
            history.push_back("No manual entry for: " + args);
        }
    });
    
    AddCommand("run", [&](const std::string& args) {    
        if (fileSystem.find(args) != fileSystem.end()) {
            size_t dotPos = args.find_last_of('.');
            if (dotPos == std::string::npos) {
                return;
            }
            string extension = args.substr(dotPos + 1);
            if(extension != "lua"){
                history.push_back("File is not excutable : " + args);
                PlaySound(s_error);
            }
            else if (fileSystem[args].second.find('x') == std::string::npos) {
                history.push_back("Permission denied: " + args);
                PlaySound(s_error);
            } else {
                history.push_back("script.lua ...");
            }
        } else {
            history.push_back("No such file: " + args);
        }
    });
    AddCommand("nano", [&](const std::string& args) {
        if (args.empty()) {
            history.push_back("Usage: nano <filename>");
            return;
        }
    
        std::string& content = fileSystem[args].first;
        std::string& permissions = fileSystem[args].second;
    
        if (permissions.find('w') == std::string::npos) {
            history.push_back("Permission denied: " + args);
            return;
        }
    
        currentEditingFile = args;
        inNanoMode = true;
        nanoBuffer.clear();
    
        history.push_back("Editing file: " + args);
        history.push_back("Add <text>' to add new lines.");
        history.push_back("Or use '<line number> <text>' to edit lines.");
        history.push_back("Type ':wq' to save and exit. ':q' to quit without saving.");
    
        if (!content.empty()) {
            std::istringstream stream(content);
            std::string line;
            int lineNumber = 1;
            while (std::getline(stream, line)) {
                nanoBuffer.push_back(line);
                history.push_back(std::to_string(lineNumber++) + " | " + line);
            }
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
    if (inNanoMode) {
        if (input == ":wq") {
            std::string result;
            for (const std::string& line : nanoBuffer) {
                result += line + "\n";
            }
            result.pop_back(); // remove trailing newline
            fileSystem[currentEditingFile].first = result;
            history.push_back("Saved and exited nano: " + currentEditingFile);
            inNanoMode = false;
            currentEditingFile.clear();
            nanoBuffer.clear();
            return;
        } else if (input == ":q") {
            history.push_back("Exited nano without saving: " + currentEditingFile);
            inNanoMode = false;
            currentEditingFile.clear();
            nanoBuffer.clear();
            return;
        }
    
        // Check for line number edit: "2 Hello World"
        if (!input.empty() && std::isdigit(input[0])) {
            size_t space = input.find(' ');
            if (space != std::string::npos) {
                std::string lineNumStr = input.substr(0, space);
                std::string lineContent = input.substr(space + 1);
    
                try {
                    int lineNum = std::stoi(lineNumStr);
                    if (lineNum >= 1 && lineNum <= (int)nanoBuffer.size()) {
                        nanoBuffer[lineNum - 1] = lineContent;
                        history.push_back("Edited line " + std::to_string(lineNum));
                    } else {
                        history.push_back("Line number out of range.");
                    }
                } catch (...) {
                    history.push_back("Invalid line number.");
                }
            } else {
                history.push_back("To edit a line: <line number> <new text>");
            }
        } else {
            nanoBuffer.push_back(input);
        }
    
        return;
    }
    

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

// Files

void Terminal::AddFile(const std::string& path, const std::string& content, const std::string& permission) {
    fileSystem[path] = { content, permission };
}

void Terminal::ClearFiles() {
    fileSystem.clear();
}
