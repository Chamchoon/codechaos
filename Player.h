#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <vector>
#include "Wall.h"

class Player {
public:
    Vector2 position;
    float speed;
    float radius;
    float velocityY;
    bool isOnGround;
    Color color;

    Player(float x, float y);
    void Update(const std::vector<Wall>& walls);
    void Draw();
};

#endif
