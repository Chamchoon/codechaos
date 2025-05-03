#include "Switch.h"


Switch::Switch(float x, float y) {
    rect = { x, y, 30, 30};
    isActive = false;
    activeColor = YELLOW;
    inactiveColor = GRAY;
    s_switch = LoadSound("../sounds/switch.mp3");
}


void Switch::Toggle() {
    isActive = !isActive;
    PlaySound(s_switch);
}

void Switch::Draw() {
    DrawRectangleRec(rect, isActive ? activeColor : inactiveColor);
}

void Switch::Update() {
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, rect)) {
        Toggle();
    }
}
