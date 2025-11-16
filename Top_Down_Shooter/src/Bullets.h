#pragma once
#include "raylib.h"


class Bullet {
public:
    Vector2 position;   
    Vector2 velocity;   
    float bulletspeed;        
    float lifetime;     
    bool active;        
	int damage;
	 

    Bullet(Vector2 startPos, float angle, float bspeed, float life, int dmg);

	
    void Update(float dt);
    void Draw() const;
    float GetSpeed();
    void SetSpeed(float newSpeed);
    Vector2 GetCenter() const;
    float GetRadius() const;
};
