#include "Door.h"
#include "Player.h"
#include <iostream>

Door::Door() {
    rect = {750, 450, 50, 100};
    solvedColor = GREEN;
    normalColor = GRAY;
    opened = false;    
    s_openDoor = LoadSound("../sounds/openDoor.wav");
}

void Door::openDoor(){
    if(!opened){
        opened = true;
        PlaySound(s_openDoor);
    }
    
}

void Door::Draw() {
    DrawRectangleRec(rect, opened ? solvedColor : normalColor);
}

bool Door::CheckCollision(const Vector2& player) {
    return CheckCollisionPointRec(player, rect);
}
