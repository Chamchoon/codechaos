#include "Player.h"
#include <iostream>
#include "Wall.h"
#include <cmath>
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
            // Only block X if we're not standing on the wall (vertical overlap matters)
            float playerBottom = position.y + radius;
            float wallTop = wall.rect.y;
    
            if (playerBottom <= wallTop + 1.0f) { // slight buffer
                // Standing on it or nearly, don't block X
                continue;
            }
    
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
            if (velocityY > 0) {
                // Falling down, align player to top of wall
                position.y = wall.rect.y - radius;
                isOnGround = true;
            } else if (velocityY < 0) {
                // Hitting ceiling, align to bottom
                position.y = wall.rect.y + wall.rect.height + radius;
            }
            velocityY = 0;
            break;
        }
    }
    
    if (canMoveY) position.y += velocityY;
    if (std::isnan(position.y) || std::isinf(position.y) || position.y > 10000.0f) {
        std::cout << "Invalid Y detected: " << position.y << "\n";
        position = {50, 50};
        velocityY = 0;
    }
    
}

void Player::Draw() {
    DrawCircleV(position, radius, color);
}