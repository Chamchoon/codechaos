extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}
#include "LevelLoader.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

// Define the static variable
json LevelLoader::lastLoadedJson;
bool movingWall = false;
lua_State* L = luaL_newstate();


bool LevelLoader::LoadLevel(const std::string& path, Level& l) {
    l.Clear();
    luaL_openlibs(L);
    movingWall = false;
    l.player.position.y = 250;
    l.player.position.x = 50;

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
        l.dynamicWalls.emplace_back(w[0]["x"], w[0]["y"], w[0]["width"], w[0]["height"]);
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
    if (j.contains("door")){
        l.door.rect.x = j["door"]["x"];
        l.door.rect.y = j["door"]["y"];
        l.door.opened = j["door"]["opened"];
    }else{
        l.door.rect.x = 750;
        l.door.rect.y = 250;
        l.door.opened = false;
    }
    
    

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
    luaL_openlibs(L);
    if (!lastLoadedJson.contains("type")) return false;

    string type = lastLoadedJson["type"];
    
    if(movingWall==true){       
        int targetY = lastLoadedJson["dynamicWalls"][0][1]["y"];
    
        if (l.dynamicWalls.at(0).rect.y < targetY - 0.01f) {
            l.dynamicWalls.at(0).rect.y += 1;
            if (l.dynamicWalls.at(0).rect.y > targetY) l.dynamicWalls.at(0).rect.y = targetY;
        } else if (l.dynamicWalls.at(0).rect.y > targetY + 0.01f) {
            l.dynamicWalls.at(0).rect.y -= 1;
            if (l.dynamicWalls.at(0).rect.y < targetY) l.dynamicWalls.at(0).rect.y = targetY;
        }
    }

    if (type == "Terminal Output") {
        if (!l.terminal.history.empty() && l.terminal.history.back() == lastLoadedJson["output"]) {
            l.door.openDoor();
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
        if (lastLoadedJson.contains("dynamicWalls")) {
            movingWall = true;
            return true;
        }else{
            l.door.openDoor();
            return true;
        }
        
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
        l.door.openDoor();
        return true;
    }else if(type=="Programming"){
        string lua_script = l.terminal.fileSystem["script.lua"].first;
        if(!l.terminal.history.empty() && l.terminal.history.back() == "script.lua ..."){
            lua_State* L = luaL_newstate();
            luaL_openlibs(L);
            for (const auto& var : lastLoadedJson["variables"]) {
                std::string varName = var["name"];
                std::string type = var["type"];
        
                if (varName == "movingWall" && type == "boolean") {
                    lua_pushboolean(L, movingWall);
                }else if (varName == "OpenTheDoor" && type == "boolean")
                {
                    lua_pushboolean(L, l.door.opened);
                }else if (varName == "dynamicWallY" && type == "float") {
                    lua_pushinteger(L, l.dynamicWalls.at(0).rect.y);
                }else if (varName == "dynamicWallX" && type == "float") {
                    lua_pushinteger(L, l.dynamicWalls.at(0).rect.x);
                }
        
                lua_setglobal(L, varName.c_str());
            }
            if (luaL_dostring(L, lua_script.c_str()) != LUA_OK) {
                l.terminal.history.push_back("Lua Error: " + (string)lua_tostring(L, -1));
                lua_close(L);
                return false;
            }
            for (const auto& var : lastLoadedJson["variables"]) {
                std::string varName = var["name"];
                std::string type = var["type"];
        
                lua_getglobal(L, varName.c_str());
        
                if (varName == "movingWall" && type == "boolean" && lua_isboolean(L, -1)) {
                    movingWall = lua_toboolean(L, -1);
                }else if (varName == "OpenTheDoor" && type == "boolean")
                {
                    l.door.opened = lua_toboolean(L, -1);
                }else if (varName == "dynamicWallY" && type == "float" && lua_isnumber(L, -1)) {
                    l.dynamicWalls.at(0).rect.y = lua_tointeger(L, -1);
                }else if (varName == "dynamicWallX" && type == "float" && lua_isnumber(L, -1)) {
                    l.dynamicWalls.at(0).rect.x = lua_tointeger(L, -1);
                }
        
                lua_pop(L, 1);
            }
            lua_close(L);
            return true;
        }
    }
    if (lastLoadedJson["type"] == "PublicIP") {
        const string targetIP = "192.225.83.117";
        l.terminal.AddCommand("ip", [&](const string& args){
            if (args == targetIP) l.door.openDoor();
        });
        l.terminal.EnableCommand("ip");
    }
    return false;
}
