#ifndef DOOR_H
#define DOOR_H

#include "raylib.h"
#include "Player.h"

using namespace std;
class Door {
public:
    Rectangle rect;
    bool opened;
    Color solvedColor;
    Color normalColor;

    Door();
    void Draw();
    bool CheckCollision(const Vector2& player);
};

#endif