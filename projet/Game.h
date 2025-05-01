#ifndef GAME_H
#define GAME_H

#include "Menu.h"
#include "raylib.h"

// Declare all SFX and music as extern so they can be used across files
extern Sound s_ballJump;
extern Sound s_buttonClick;
extern Sound s_error;
extern Sound s_hintReveal;
extern Sound s_switch;
extern Sound s_popUp;
extern Sound s_popDown;
extern Sound s_openDoor;
extern Sound s_youWin;
extern Sound s_nextLevel;
extern Sound s_toMenu;
extern Music bgm_menuLevels;

class Game {
    Menu menu;
    double startTime;
    int currentStage;
    int lastScore;
    Rectangle backBtn;
public:
    void Run();
    void PlayStage(int stage);
    void DrawWinScreen();
};

#endif // GAME_H
