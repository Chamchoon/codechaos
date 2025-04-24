#include "Door.h"
#include "Player.h"

Door::Door() {
    rect = { 750, 250, 50, 100};
    solvedColor = GREEN;
    normalColor = GRAY;
    opened = false;
}



void Door::Draw() {
    DrawRectangleRec(rect, opened ? solvedColor : normalColor);
}

bool Door::CheckCollision(const Vector2& player) {
    return CheckCollisionPointRec(player, rect);
}
