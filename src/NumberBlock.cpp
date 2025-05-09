#include "NumberBlock.h"
#include <string>


NumberBlock::NumberBlock(float x, float y,int v){
    rect = { x, y, 50, 50};
    value = v;
    s_up = LoadSound("../sounds/pop-up.mp3");
    s_down = LoadSound("../sounds/pop-down.mp3");
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
        PlaySound(s_up);
    }else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && CheckCollisionPointRec(mousePos, rect)){
        value--;
        PlaySound(s_down);
    }
}