#include "Bullets.h"
#include "raylib.h"
#include <cmath>

Bullet::Bullet(Vector2 startPos, float angle, float bspeed, float life, int dmg)
    : position(startPos), lifetime(life), damage(dmg), active(true)
{
    bulletspeed = bspeed;
    velocity.x = cosf(angle) * bulletspeed;
    velocity.y = sinf(angle) * bulletspeed;
   
}

void Bullet::Update(float dt) {
    if (!active) return;

    // Move
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;

    
    lifetime -= dt;
    if (lifetime <= 0.0f) {
        active = false;
        return;
    }

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    const float radius = 5.0f; 

    if (position.x < -radius || position.x > screenWidth + radius ||
        position.y < -radius || position.y > screenHeight + radius) {
        active = false;
    }
}

void Bullet::Draw() const {
    if (active)
        DrawCircleV(position, 1.5, YELLOW);
}

float Bullet::GetSpeed() {
	return bulletspeed;
}

void Bullet::SetSpeed(float newSpeed) {
	bulletspeed += newSpeed;
}

Vector2 Bullet::GetCenter() const {
    return position;
}

float Bullet::GetRadius() const {
    return 5.0f;
}
