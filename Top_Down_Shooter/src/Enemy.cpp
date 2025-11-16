#include "Enemy.h"
#include "raymath.h"
#include <queue>
#include <map>
#include <cmath>



Enemy::Enemy(Vector2 pos, float r, int hp)
{
    position = pos;
    radius = r;
    health = hp;
    alive = true;
    if (s_frames.empty()) {
        for (int i = 1; i <= 8; i++) {
            s_frames.push_back(LoadTexture(TextFormat("%s/Enemy/Bat%d.png", RESOURCES_PATH, i)));
        }
    }

    // Use static textures
    frames = s_frames;
    currentFrame = 0;
    frameTimer = 0.0f;

}

std::vector<Texture2D> Enemy::s_frames;

void Enemy::TakeDamage(int dmg)
{
    health -= dmg;
    if (health <= 0) alive = false;
}

bool Enemy::IsCollidingWall(Vector2 pos, const std::vector<Rectangle>& walls)
{
    for (const auto& w : walls) {
        float closestX = fmaxf(w.x, fminf(pos.x, w.x + w.width));
        float closestY = fmaxf(w.y, fminf(pos.y, w.y + w.height));
        float dx = pos.x - closestX;
        float dy = pos.y - closestY;
        if ((dx * dx + dy * dy) < (radius * radius)) return true;
    }
    return false;
}

void Enemy::Update(float dt, Vector2 playerPos)
{
    if (!alive) return;

    
    // Movement
    Vector2 dir = Vector2Normalize(Vector2Subtract(playerPos, position));
    position = Vector2Add(position, Vector2Scale(dir, speed * dt));
    velocity = Vector2Scale(dir, speed);
    // Animation using delta time (smooth, no skipped frames)
    frameTimer += dt;

    while (frameTimer >= frameTime)
    {
        frameTimer -= frameTime;              // subtract, don't reset
        currentFrame = (currentFrame + 1) % frames.size(); // loop frames
    }
}

void Enemy::Draw() const
{
    if (!alive) return;

    Texture2D tex = frames[currentFrame];
    float scale = 0.3f;

    // Determine horizontal flip
    bool flipLeft = velocity.x < 0;

    Rectangle sourceRec = { 0, 0, (float)tex.width * (flipLeft ? -1 : 1), (float)tex.height };
    Rectangle destRec = { position.x, position.y, tex.width * scale, tex.height * scale };
    Vector2 origin = { tex.width * scale / 2, tex.height * scale / 2 };

    DrawTexturePro(tex, sourceRec, destRec, origin, 0.0f, WHITE);
}

struct Node {
    int x, y;
    float f, g, h;
    Node* parent;
    Node(int x_, int y_) : x(x_), y(y_), f(0), g(0), h(0), parent(nullptr) {}
};
