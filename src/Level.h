// Level.h
#pragma once
#include <vector>
#include "Wall.h"
#include "Player.h"
#include "NumberBlock.h"
#include "Switch.h"
#include "Door.h"
#include "Terminal.h"
#include <string>
using namespace std;

struct Level {
    std::vector<Wall> walls;
    std::vector<NumberBlock> numberBlocks;
    std::vector<Switch> Switches;
    std::string text;
    int textAttributes[3];
    Door door;
    Player player;
    Terminal terminal;
    
    void Clear();  // to clear/reset level objects before loading another
};
