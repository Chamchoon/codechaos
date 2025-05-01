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

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_TAB)) level.terminal.Toggle();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        LevelLoader::CheckEvent(level);
        std::vector<Wall> allWalls;
        allWalls.reserve(level.walls.size() + level.dynamicWalls.size());

        if (!level.terminal.isOpen) {            
            
            allWalls.clear();
            allWalls.insert(allWalls.end(), level.walls.begin(), level.walls.end());
            allWalls.insert(allWalls.end(), level.dynamicWalls.begin(), level.dynamicWalls.end());
            level.player.Update(allWalls);
            level.player.Draw();
            cout<<"x : "<<level.player.position.x<<endl;

            
            
            
            for (Wall& wall : level.walls) wall.Draw();
            for (Wall& dywall : level.dynamicWalls) dywall.Draw();
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
            if(level.player.position.y<0 || level.player.position.y>600){
                level.player = Player(50,50);
                level.player.velocityY = 0;
                level.player.isOnGround = false;
            }
        }
        
        //cout<<level.player.position.y<<endl;
        level.terminal.Draw();
        level.terminal.Update();
        

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
