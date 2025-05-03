#ifndef STAGEBUTTON_H
#define STAGEBUTTON_H

#include "raylib.h"

class StageButton {
public:
    Rectangle rect;
    Rectangle reloadRect;
    int index;
    int score;
    bool unlocked;

    StageButton(int idx, float x, float y);
    void Draw();
    bool Clicked(const Vector2& mousePos);
    bool ReloadClicked(const Vector2& mousePos);
};

#endif