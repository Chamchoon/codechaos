#include "StageButton.h"
#include "ScoreManager.h"

StageButton::StageButton(int idx, float x, float y) {
    index = idx;
    rect = { x, y, 120, 60 };
    reloadRect = { x + 100, y, 20, 20 };
    score = ScoreManager::Get(idx);
    unlocked = (idx == 1) || (ScoreManager::Get(idx-1) > 0);
}

void StageButton::Draw() {
    DrawRectangleRec(rect, unlocked ? LIGHTGRAY : DARKGRAY);
    DrawText(TextFormat("Stage %d", index), rect.x + 10, rect.y + 10, 20, BLACK);
    DrawText(TextFormat("%d", score), rect.x + 10, rect.y + 35, 20, BLACK);
    // show reload icon only if this stage has been completed
    if (score > 0) DrawRectangleRec(reloadRect, SKYBLUE);
}

bool StageButton::Clicked(const Vector2& mousePos) {
    return unlocked && CheckCollisionPointRec(mousePos, rect);
}

bool StageButton::ReloadClicked(const Vector2& mousePos) {
    return (score > 0) && CheckCollisionPointRec(mousePos, reloadRect);
}
