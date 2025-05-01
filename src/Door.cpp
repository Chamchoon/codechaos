#include "Door.h"
#include "Player.h"


Door::Door() {
    rect = {750, 450, 50, 100};
    solvedColor = GREEN;
    normalColor = GRAY;
    opened = false;
    s_openDoor = LoadSound("../sounds/openDoor.wav");
}
Door::Door(float x,float y,bool v) {
    rect = {x, y, 50, 100};
    solvedColor = GREEN;
    normalColor = GRAY;
    opened = v;
    s_openDoor = LoadSound("sounds/openDoor.wav");
}

void Door::openDoor(){
    opened = true;
    PlaySound(s_openDoor);
}

void Door::Draw() {
    DrawRectangleRec(rect, opened ? solvedColor : normalColor);
}

bool Door::CheckCollision(const Vector2& player) {
    return CheckCollisionPointRec(player, rect);
}
