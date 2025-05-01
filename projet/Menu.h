#include "ScoreManager.h"
#include "raylib.h"

int ScoreManager::Get(int stage) {
    return LoadStorageValue(stage);
}

void ScoreManager::Set(int stage, int score) {
    int best = Get(stage);
    if (score > best) SaveStorageValue(stage, score);
}