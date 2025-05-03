#ifndef NUMBERBLOCK_H
#define NUMBERBLOCK_H

#include "raylib.h"

class NumberBlock {
public:
    Rectangle rect;
    Sound s_up;
    Sound s_down;
    int value;
    NumberBlock(float x, float y,int v);


    void Draw();
    void Update();
};

#endif
