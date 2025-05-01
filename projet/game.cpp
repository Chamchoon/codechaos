#include "Game.h"
#include "ScreenManager.h"
#include "ScoreManager.h"

// Define all sound/music variables
Sound s_ballJump;
Sound s_buttonClick;
Sound s_error;
Sound s_hintReveal;
Sound s_switch;
Sound s_popUp;
Sound s_popDown;
Sound s_openDoor;
Sound s_youWin;
Sound s_nextLevel;
Sound s_toMenu;
Music bgm_menuLevels;

void Game::Run() {
    InitWindow(800, 600, "Puzzle Game");
    InitAudioDevice();  // initialize audio system 

    // Load SFX from sounds/ directory :contentReference[oaicite:2]{index=2}
    s_ballJump    = LoadSound("sounds/ballJump.wav");
    s_buttonClick = LoadSound("sounds/bouttonClick.wav");
    s_error       = LoadSound("sounds/error.wav");
    s_hintReveal  = LoadSound("sounds/HintReveal.mp3");
    s_switch      = LoadSound("sounds/switch.mp3");
    s_popUp       = LoadSound("sounds/pop-up.mp3");
    s_popDown     = LoadSound("sounds/pop-down.mp3");
    s_openDoor    = LoadSound("sounds/openDoor.wav");
    s_youWin      = LoadSound("sounds/youWin.mp3");
    s_nextLevel   = LoadSound("sounds/nextLevel.mp3");
    s_toMenu      = LoadSound("sounds/toMenu.wav");

    // Load & start background music :contentReference[oaicite:3]{index=3}
    bgm_menuLevels = LoadMusicStream("sounds/menu&levels.mp3");
    PlayMusicStream(bgm_menuLevels);

    SetTargetFPS(60);
    backBtn = { 300, 350, 200, 50 };

    while (!WindowShouldClose()) {
        UpdateMusicStream(bgm_menuLevels);  // keep music playing :contentReference[oaicite:4]{index=4}

        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (ScreenManager::current) {
            case Screen::MENU:
                menu.Update();
                menu.Draw();
                menu.StageChosen(currentStage);
                break;
            case Screen::PLAYING:
                PlayStage(currentStage);
                break;
            case Screen::WIN_SCREEN:
                DrawWinScreen();
                break;
        }
        EndDrawing();
    }

    // Unload all sounds & music, then close audio 
    UnloadSound(s_ballJump);
    UnloadSound(s_buttonClick);
    UnloadSound(s_error);
    UnloadSound(s_hintReveal);
    UnloadSound(s_switch);
    UnloadSound(s_popUp);
    UnloadSound(s_popDown);
    UnloadSound(s_openDoor);
    UnloadSound(s_youWin);
    UnloadSound(s_nextLevel);
    UnloadSound(s_toMenu);
    UnloadMusicStream(bgm_menuLevels);
    CloseAudioDevice();
    CloseWindow();
}

void Game::PlayStage(int stage) {
    startTime = GetTime();  // start timer :contentReference[oaicite:5]{index=5}
    bool completed = false;

    while (!completed && !WindowShouldClose()) {
        // placeholder: SPACE completes stage & jump SFX
        if (IsKeyPressed(KEY_SPACE)) {
            completed = true;
            PlaySound(s_ballJump);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(TextFormat("Stage %d", stage), 200, 180, 30, BLACK);
        DrawText("Press SPACE to complete", 200, 220, 20, DARKGRAY);
        EndDrawing();
    }

    // compute and store score
    lastScore = (int)((100.0 - (GetTime() - startTime)) * 10);
    ScoreManager::Set(stage, lastScore);

    // play win & next-level sounds :contentReference[oaicite:6]{index=6}
    PlaySound(s_youWin);
    PlaySound(s_nextLevel);

    ScreenManager::Change(Screen::WIN_SCREEN);
}

void Game::DrawWinScreen() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("You Win!", 320, 200, 40, GREEN);
    DrawText(TextFormat("Score: %d", lastScore), 340, 260, 30, BLACK);
    DrawRectangleRec(backBtn, LIGHTGRAY);
    DrawText("Back to Menu", backBtn.x + 20, backBtn.y + 10, 20, BLACK);
    EndDrawing();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(GetMousePosition(), backBtn)) {
        PlaySound(s_toMenu);
        ScreenManager::Change(Screen::MENU);
    }
}
