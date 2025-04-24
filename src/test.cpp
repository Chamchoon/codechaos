#include "raylib.h"
#include "Level.h"
#include "LevelLoader.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    InitWindow(800, 600, "Level-Based Game");
    SetTargetFPS(60);

    
    Level level;
    LevelLoader::LoadLevel("../Levels/Level1.json", level);
    int currentLevel = 1;
    cout<<level.terminal.commandMap.size()<<endl;
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_TAB)) level.terminal.Toggle();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!level.terminal.isOpen) {
            LevelLoader::CheckEvent(level);
            level.player.Update(level.walls);
            level.player.Draw();

            for (Wall& wall : level.walls) wall.Draw();
            for (auto& nb : level.numberBlocks){
                nb.Draw();
                nb.Update();
            }
            for (auto& s : level.Switches){
                s.Draw();
                s.Update();
            } 
            if(level.text != ""){
                DrawText(level.text.c_str(), level.textAttributes[0], level.textAttributes[1], level.textAttributes[2], DARKGRAY);
            }
            level.door.Draw();

            if (level.door.CheckCollision(level.player.position) && level.door.opened) {
                currentLevel++;
                std::cout << "You win!" << std::endl;
                std::string path = "../Levels/Level" + std::to_string(currentLevel) + ".json";
                LevelLoader::LoadLevel(path, level);
            }
        }

        level.terminal.Draw();
        level.terminal.Update();
        

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
