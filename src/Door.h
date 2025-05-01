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
    Door(float x,float y,bool v);
    void Draw();
    void openDoor();
    bool CheckCollision(const Vector2& player);
    Sound s_openDoor;
};

#endif