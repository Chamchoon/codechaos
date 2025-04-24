// Level.cpp
#include "Level.h"

void Level::Clear() {
    walls.clear();
    numberBlocks.clear();
    Switches.clear();
    door = Door();
    player = Player(50,50);
    terminal.ClearActiveCommands();
    terminal.ClearFiles();
    terminal.history.clear();
    text = "";
}
