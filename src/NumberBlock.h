#ifndef NUMBERBLOCK_H
#define NUMBERBLOCK_H

#include "raylib.h"

class NumberBlock {
public:
    Rectangle rect;
    int value;
    NumberBlock(float x, float y,int v);


    void Draw();
    void Update();
};

#endif
