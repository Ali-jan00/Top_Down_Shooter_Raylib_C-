#pragma once
#include "raylib.h"
#include "Bullets.h"
#include <vector>
using namespace std;

enum GunType {
    pistol,
    shotgun,
    rifle,
    sniper
};

class Player {
private:
    Vector2 position;   
    float speed;        
    float angle;        
    int health;         
    int coins;   
	double cooldown = 0.2;
	double lastShotTime = 0.0; 
    float shootTimer = 0.0f;
    float survivalTime = 0.0;
    Texture2D player;
    vector<Texture2D> guns;
	vector<Texture2D> gunsFlipped;
    vector<Sound> gunshots;
    GunType currentGun = pistol; 

    struct GunPreset {
        float bulletSpeed;
        double fireRate;   
        float bulletLife;  
        int damage;       
    };

    GunPreset GetGunPreset(GunType gun) {
        switch (gun) {
        case pistol:   return { 500.0f, 0.5, 2.0f, 25 };
        case shotgun:  return { 400.0f, 1.0, 1.5f, 80 };
        case rifle:    return { 700.0f, 0.2, 3.0f, 35 };
        case sniper:   return { 1000.0f, 1.5, 5.0f, 100 };
        }
        return { 500.0f, 0.5, 2.0f, 25 }; // fallback
    }

        int currentFrame = 0;
        float frameTime = 0.1f;      
        float frameTimer = 0.0f;
        Texture2D vampire;
        Texture2D vampire2;                
        int frameCount = 6;           
        bool facingRight = true;
public:
    float damageCooldown = 0.0f;
    
    Player(Vector2 startPos, float speed = 200.0f, int health = 100);
   
    // Update and draw
    void Update(float dt, const Camera2D& camera);
    
    void DrawGun() const;
	void DrawPlayer() const;
    void Draw() const;
    // Behavior
    void Move(float dt);
    void Die();
    void Shoot(std::vector<Bullet>& bullets);


    // Getters
    Vector2 GetPosition() const;
    float GetAngle() const;
    int GetHealth() const;
	float GetSpeed() const;
	double GetCooldown() const;
    GunType GetGun() const;
    float GetSurvivalTime() const;
    // Setters
	void SetCooldown(int percent);
	void SetSpeed(float newSpeed);
    void SetPosition(Vector2 newPos);
    void SetHealth(int newHealth);
    int GetCoins() const;
    void AddCoins(int amount);
    void SpendCoins(int amount);
    void SetCoins(int amount);
    Vector2 GetCenter() const;
    float GetRadius() const;
    void SetGun(GunType gun);
	Texture2D GetCurrentGunTexture() const;
	Texture2D GetGunTexture(int i) const;
    int GetGunDamage();
};
