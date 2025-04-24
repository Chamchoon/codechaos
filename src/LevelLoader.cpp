#include "LevelLoader.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

// Define the static variable
json LevelLoader::lastLoadedJson;

bool LevelLoader::LoadLevel(const std::string& path, Level& l) {
    l.Clear();  // clear level state

    ifstream file(path);
    if (!file.is_open()) {
        cout << "Cannot load level: " << path << endl;
        return false;
    }

    json j;
    file >> j;
    lastLoadedJson = j;  // store for later use in CheckEvent

    for (const auto& w : j["walls"]) {
        l.walls.emplace_back(w["x"], w["y"], w["width"], w["height"]);
    }
    for (const auto& w : j["dynamicWalls"]) {
        l.dynamicWalls.emplace_back(w["x"], w["y"], w["width"], w["height"]);
    }
    for (const auto& b : j["numberBlocks"]) {
        l.numberBlocks.emplace_back(b["x"], b["y"],b["v"]);
    }

    for (const auto& s : j["switches"]) {
        l.Switches.emplace_back(s["x"], s["y"]);
    }

    if (j.contains("text")){
        l.text = j["text"];
        for(int i=0;i<3;i++){
            l.textAttributes[i] = j["textAttributes"][i];
        }
    }

    l.door = Door();
    

    if (j.contains("terminal")) {
        auto terminalData = j["terminal"];
        l.terminal.InitDefaultCommands();
        for (const auto& cmd : terminalData["enabledCommands"]) {
            l.terminal.EnableCommand(cmd);
        }
        for (const auto& [filename, properties] : terminalData["files"].items()) {
            l.terminal.AddFile(filename, properties[0],properties[1]);
        }
    }

    return true;
}

bool LevelLoader::CheckEvent(Level& l) {
    if (!lastLoadedJson.contains("type")) return false;

    string type = lastLoadedJson["type"];
    if (type == "Terminal Output") {
        if (!l.terminal.history.empty() && l.terminal.history.back() == lastLoadedJson["output"]) {
            l.door.opened = true;
            return true;
        }
    }else if(type=="binary"){
        vector<bool> answerFromJson = lastLoadedJson["binaryAnswer"].get<vector<bool>>();
        int i = 0;
        for (const auto& s : l.Switches) {
            if(answerFromJson.at(i) != s.isActive){
                return false;
            }
            i++;
        }
        if(lastLoadedJson.contains("dynamicWalls")){
            float f = lastLoadedJson["dynamicWalls"][0]["y"];
            while(f<lastLoadedJson["finalWalls"][0]){
                f += 0.2;
                cout<<i<<endl;
            }
        }
        l.door.opened = true;
        return true;
    }
    else if(type=="MathBlocks"){
        vector<int> answerFromJson = lastLoadedJson["MathBlocksAnswer"].get<vector<int>>();
        int i = 0;
        for (const auto& nb : l.numberBlocks) {
            if(answerFromJson.at(i) != nb.value){
                return false;
            }
            i++;
        }
        l.door.opened = true;
        return true;
    }
    return false;
}
