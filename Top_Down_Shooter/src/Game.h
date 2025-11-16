#pragma once
#include "Player.h"
#include "Bullets.h"
#include "Enemy.h"
#include <vector>

enum class GameState {
    StartMenu,
	Menu,
    Playing,
    GameOver,
    PauseMenu
};

class Game {
public:
    Camera2D camera;    
    GameState state= GameState::StartMenu;
    Game(int screenWidth, int screenHeight);
    void Update();
    void Draw();
    bool Getexitrequest();
private:
    std::vector<Texture2D> healthBars;
    Music PlayingMusic;
    Music startMenuMusic;
    bool startMenuMusicPlaying = false;
	bool IsgameMusicPlaying = false;
    bool exitRequest=false;
    Font pixel_font;
    int screenWidth, screenHeight;
	int coins;
    Player player;
    vector<Bullet> bullets;
    vector<bool> playerGuns{ true, false, false, false };

    void HandleCollisions(Vector2 prevPos);
    Texture2D background;
    Texture2D StartMenu_bg;
	Texture2D GameOverTexture;
    Texture2D ShopMenu;
	Texture2D Shop;
    Texture2D PauseMenu;
    vector<Rectangle> walls;
    std::vector<Enemy> enemies;

    int wave = 1;
    int enemiesToSpawn = 5;
    int seconds,minutes,score;

    void SpawnWave();
    void UpdateEnemies(float dt);
    void HandleEnemyCollisions();

};
