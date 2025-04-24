#include "Player.h"
#include <iostream>
#include "Wall.h"
using namespace std;
#define GRAVITY 0.5f
#define JUMP_STRENGTH 10.0f
Player::Player(float x, float y) {
    position = {x, y};
    speed = 5.0f;
    radius = 20.0f;
    isOnGround = false;
    color = BLUE;
}

bool CheckCollision(const Rectangle& player, const Wall& wall) {
    return CheckCollisionRecs(player, wall.rect);
}

void Player::Update(const std::vector<Wall>& walls) {
    float moveX = 0;
    velocityY += GRAVITY;

    if (IsKeyDown(KEY_A)) moveX = -speed;
    if (IsKeyDown(KEY_D)) moveX = speed;

    // Check collision for X movement
    Rectangle futureX = {position.x + moveX - radius, position.y - radius, radius * 2, radius * 2};
    bool canMoveX = true;
    for (const Wall& wall : walls) {
        if (CheckCollision(futureX, wall)) {
            canMoveX = false;
            break;
        }
    }
    if (canMoveX) position.x += moveX;


    if (IsKeyPressed(KEY_SPACE) && isOnGround) {
        velocityY = -JUMP_STRENGTH;
        isOnGround = false;
    }
    // Check collision for Y movement
    Rectangle futureY = {position.x - radius, position.y + velocityY - radius, radius * 2, radius * 2};
    bool canMoveY = true;
    for (const Wall& wall : walls) {
        if (CheckCollision(futureY, wall)) {
            canMoveY = false;
            velocityY = 0;
            isOnGround = true;
            break;
        }
    }
    if (canMoveY) position.y += velocityY;
}

void Player::Draw() {
    DrawCircleV(position, radius, color);
}