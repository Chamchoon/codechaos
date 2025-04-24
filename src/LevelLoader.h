#pragma once
#include "Level.h"
#include <string>
#include "nlohmann/json.hpp"

class LevelLoader {
public:
    static bool LoadLevel(const std::string& path, Level& l);
    static bool CheckEvent(Level& l);
private:
    static nlohmann::json lastLoadedJson;  // keep JSON from last loaded level
};
