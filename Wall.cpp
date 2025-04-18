#include "Wall.h"

Wall::Wall(float x, float y, float width, float height) {
    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;
    color = DARKGRAY;
}

void Wall::Draw() {
    DrawRectangleRec(rect, color);
}
