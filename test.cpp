#include "raylib.h"
#include "Player.h"
#include "Wall.h"
#include "Switch.h"
#include "Binary.h"
#include "Door.h"
#include "NumberBlock.h"
#include "Terminal.h"
#include <vector>
#include <iostream>
using namespace std;

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Simple Collision");
    SetTargetFPS(60);

    Player player(50, 50);

    Terminal terminal;
    terminal.InitDefaultCommands();
    terminal.AddCommand("help", [&](const std::string&) {
        terminal.history.push_back("Availble Commands : help echo clear.");
        cout<<terminal.history.at(terminal.history.size()-1)<<endl;
    });

    vector<string> enabledCommands = {"help","echo","clear"};
    for(string cmd:enabledCommands){
        terminal.EnableCommand(cmd);
    }

    std::vector<Wall> walls = {
        Wall(300, 300, 100, 20),
        Wall(0, 400, 500, 20)
    };
    Switch s1 = Switch(50,50);
    Switch s2 = Switch(100,50);
    bool solution[] = {true,true};
    bool answer[] = {false,false};
    NumberBlock b = NumberBlock(200,200);
    Door d = Door();
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_TAB)) terminal.Toggle();
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if(!terminal.isOpen){
            player.Update(walls);
            s1.Update();
            s1.Draw();
            s2.Update();
            s2.Draw();
            answer.at
            player.Draw();
            for (Wall& wall : walls){
                wall.Draw();
                wall.rect.width += 5;
            }
            DrawText("to see if you are a true programmer echo our first phrase\n press tab then type help to know how", 10, 10, 20, DARKGRAY);
            
            b.Update();
            b.Draw();   
            d.Draw();
            if(d.CheckCollision(player.position) && d.opened){
                cout<<"win"<<endl;
            }
        }
        
        terminal.Update();
        terminal.Draw();
        if(terminal.history.size()>0){
            if(terminal.history.at(terminal.history.size()-1)=="hello world"){
                d.opened = true;
            }
        }
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
