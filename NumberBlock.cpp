#include "NumberBlock.h"
#include <string>


NumberBlock::NumberBlock(float x, float y){
    rect = { x, y, 50, 50};
    value = 0;
}

void NumberBlock::Draw(){
    DrawRectangleRec(rect, RAYWHITE);
    DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, BLACK);
    std::string text = std::to_string(value);
    int fontSize = 20;
    int textWidth = MeasureText(text.c_str(), fontSize);
    int textX = rect.x + (rect.width - textWidth) / 2;
    int textY = rect.y + (rect.height - fontSize) / 2;

    DrawText(text.c_str(), textX, textY, fontSize, BLACK);
}

void NumberBlock::Update() {
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, rect)) {
        value++;
    }else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && CheckCollisionPointRec(mousePos, rect)){
        value--;
    }
}