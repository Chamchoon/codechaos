#ifndef SWITCH_H
#define SWITCH_H

#include "raylib.h"
#include <vector>
using namespace std;
class Switch {
public:
    Rectangle rect;
    bool isActive;
    Color activeColor;
    Color inactiveColor;
    Sound s_switch;

    Switch(float x, float y);
    void Toggle();
    void Draw();
    void Update();
};

#endif