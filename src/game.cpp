#include "Level.h"
#include "LevelLoader.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const std::string SAVE_FILE = "save.txt";

int LoadMaxUnlockedLevel() {
    std::ifstream file(SAVE_FILE);
    int level = 1;
    if (file.is_open()) {
        file >> level;
        file.close();
    }
    return std::max(1, level);
}

void SaveMaxUnlockedLevel(int level) {
    std::ofstream file(SAVE_FILE);
    if (file.is_open()) {
        file << level;
        file.close();
    }
}


int main() {
    enum GameState {
        MENU,
        LEVELS,
        PLAYING
    };    
    GameState gameState = MENU;
    
    InitWindow(800, 600, "Level-Based Game");
    SetTargetFPS(60);
    InitAudioDevice();

    
    Level level;
    LevelLoader::LoadLevel("../Levels/Level1.json", level);
    Sound s_win = LoadSound("../sounds/nextLevel.mp3");
    Music bgMusic = LoadMusicStream("../sounds/levels.mp3");
    Music MenuMusic = LoadMusicStream("../sounds/menu.mp3");
    bgMusic.looping = true;
    float volume = 0.35f;
    SetMusicVolume(bgMusic, volume);
    PlayMusicStream(bgMusic);
    PlayMusicStream(MenuMusic);

    int currentLevel = 1;
    int maxUnlockedLevel = LoadMaxUnlockedLevel();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        
        if (gameState == MENU) {
            UpdateMusicStream(MenuMusic);
            ClearBackground(RAYWHITE);
            DrawText("CodeChaos", 300, 100, 40, DARKGRAY);
        
            Rectangle playBtn = { 320, 250, 160, 50 };
            Rectangle exitBtn = { 320, 320, 160, 50 };
            Vector2 mouse = GetMousePosition();
        
            DrawRectangleRec(playBtn, LIGHTGRAY);
            if (CheckCollisionPointRec(mouse, playBtn)) {
                DrawRectangleLinesEx(playBtn, 3, DARKGRAY);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    gameState = LEVELS;
                }
            }
            DrawText("Play", playBtn.x + 60, playBtn.y + 15, 20, DARKGRAY);
        
            // Draw Exit Button
            DrawRectangleRec(exitBtn, LIGHTGRAY);
            if (CheckCollisionPointRec(mouse, exitBtn)) {
                DrawRectangleLinesEx(exitBtn, 3, DARKGRAY);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    UnloadMusicStream(bgMusic);
                    CloseAudioDevice();
                    CloseWindow();
                    return 0;
                }
            }
            DrawText("Exit", exitBtn.x + 60, exitBtn.y + 15, 20, DARKGRAY);
        }else if (gameState == LEVELS) {
            UpdateMusicStream(MenuMusic);
            DrawText("Select a Level", 280, 50, 30, DARKGRAY);
        
            int btnWidth = 100;
            int btnHeight = 50;
            int spacing = 20;
            int startX = 150;
            int startY = 150;
        
            Vector2 mouse = GetMousePosition();
        
            for (int i = 1; i <= maxUnlockedLevel; i++) {
                int x = startX + ((i - 1) % 4) * (btnWidth + spacing);
                int y = startY + ((i - 1) / 4) * (btnHeight + spacing);
                Rectangle btn = { (float)x, (float)y, (float)btnWidth, (float)btnHeight };
        
                DrawRectangleRec(btn, LIGHTGRAY);
                if (CheckCollisionPointRec(mouse, btn)) {
                    DrawRectangleLinesEx(btn, 3, DARKGRAY);
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        currentLevel = i;
                        std::string path = "../Levels/Level" + std::to_string(currentLevel) + ".json";
                        LevelLoader::LoadLevel(path, level);
                        gameState = PLAYING;
                    }
                }
        
                DrawText(("Level " + std::to_string(i)).c_str(), x + 15, y + 15, 20, DARKGRAY);
            }
        }
        
        else if (gameState == PLAYING) {
            UpdateMusicStream(bgMusic);
            if (IsKeyPressed(KEY_TAB)) level.terminal.Toggle();
    
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
                    if (currentLevel > maxUnlockedLevel) {
                        maxUnlockedLevel = currentLevel;
                        SaveMaxUnlockedLevel(maxUnlockedLevel);
                    }
                    PlaySound(s_win);
                    std::string path = "../Levels/Level" + std::to_string(currentLevel) + ".json";
                    LevelLoader::LoadLevel(path, level);
                }
                if(level.player.position.y<0 || level.player.position.y>600){
                    level.player = Player(50,250);
                    level.player.velocityY = 0;
                    level.player.isOnGround = false;
                }
            }
    
            level.terminal.Draw();
            level.terminal.Update();
        }
    
        EndDrawing();
    }
    
    UnloadMusicStream(bgMusic);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
