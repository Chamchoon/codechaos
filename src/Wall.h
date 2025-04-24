#ifndef WALL_H
#define WALL_H

#include "raylib.h"

class Wall {
public:
    Rectangle rect;
    Color color;

    Wall(float x, float y, float width, float height);
    void Draw();
};

#endif
