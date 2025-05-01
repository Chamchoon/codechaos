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
    std::vector<Wall> dynamicWalls;
    std::vector<NumberBlock> numberBlocks;
    std::vector<Switch> Switches;
    std::string text;
    int textAttributes[3];
    Door door;
    Player player = Player(50,50);
    Terminal terminal;
    
    Level() : dynamicWalls() {}
    void Clear();  // to clear/reset level objects before loading another
};
