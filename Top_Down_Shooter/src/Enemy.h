#pragma once
#include "raylib.h"
#include <vector>

class Enemy {
private:
    Vector2 position;
    float radius;
    int health;
    bool alive;
    Vector2 velocity;
    std::vector<Vector2> path;   
    float pathTimer = 0.0f;      
    float pathCooldown = 2.0f;   
    float speed = 100.0f;
    Texture2D enemy_Texture;
    std::vector<Texture2D> frames;
    int currentFrame = 0;
    float frameTime = 0.12f;
    float frameTimer = 0.0f;
    static std::vector<Texture2D> s_frames;
public:
    Enemy(Vector2 pos, float r, int hp);

    void Update(float dt, Vector2 playerPos);
    void Draw() const;

    Vector2 GetCenter() const { return position; }
    float GetRadius() const { return radius; }
    void SetPosition(Vector2 newPos) { position = newPos; }
    void TakeDamage(int dmg);
    bool IsAlive() const { return alive; }

private:
    bool IsCollidingWall(Vector2 pos, const std::vector<Rectangle>& walls);
    std::vector<Vector2> AStarPath(Vector2 start, Vector2 goal, const std::vector<Rectangle>& walls, int cellSize);
};
