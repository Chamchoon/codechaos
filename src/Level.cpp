// Level.cpp
#include "Level.h"



void Level::Clear() {
    walls.clear();
    numberBlocks.clear();
    Switches.clear();
    dynamicWalls.clear();
    terminal.ClearActiveCommands();
    terminal.ClearFiles();
    terminal.history.clear();
    text = "";
}
