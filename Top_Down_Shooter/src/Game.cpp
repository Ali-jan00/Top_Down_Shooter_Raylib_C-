#include "Game.h"
#include "utils.h"
#include "Bullets.h"
#include "Player.h"
#include "raylib.h"
#include <algorithm>
using namespace std;

Game::Game(int screenWidth, int screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight)
    ,player({ (float)screenWidth / 2, (float)screenHeight / 2 }) {
    //backgrounds
     background = LoadTexture(TextFormat("%s/background/back.png", RESOURCES_PATH));
	 StartMenu_bg = LoadTexture(TextFormat("%s/background/StartMenu.png", RESOURCES_PATH));
	 pixel_font = utils::LoadFontFromFile("%s/Pixeled.ttf");
	 GameOverTexture = LoadTexture(TextFormat("%s/background/DiedScreen.png", RESOURCES_PATH));
	 ShopMenu = LoadTexture(TextFormat("%s/background/Shop.png", RESOURCES_PATH));
	 Shop = LoadTexture(TextFormat("%s/background/Weapons_Merchant.png", RESOURCES_PATH));
     PauseMenu = LoadTexture(TextFormat("%s/background/Pause.png", RESOURCES_PATH));
     for (int i = 0; i <= 3; i++) {
         Texture2D tex = LoadTexture(TextFormat("%s/Player/health_%d.png", RESOURCES_PATH, i));
         healthBars.push_back(tex);
     }
     //Musix
     startMenuMusic = LoadMusicStream(TextFormat("%s/Music/MainMenu.ogg", RESOURCES_PATH));
	 PlayingMusic = LoadMusicStream(TextFormat("%s/Music/GameMusic.ogg", RESOURCES_PATH));
     //Camera
     camera.target = player.GetPosition();
     camera.offset = { (float)screenWidth / 2, (float)screenHeight / 2 };
     camera.rotation = 0.0f;
     camera.zoom = 2.0f;
     float bw = (float)background.width;
     float bh = (float)background.height;
     float thickness = 50.0f;
     //collider walls
     walls = {
         { 0, 20, bw, thickness },                // top
         { 450, 0, thickness, bh },                // left
         { 2550, 0, thickness, bh },   // right
         { 0, bh - thickness-450, bw, thickness } , // bottom
         {
            530.0f,              // x
            130.0f,              // y
            (float)Shop.width-50,   // width
            (float)Shop.height   // height
          }
     };
     
}

void Game::Update() {

    float dt = GetFrameTime();
    UpdateMusicStream(startMenuMusic);
    UpdateMusicStream(PlayingMusic);
    switch (state) {
    case GameState::StartMenu:
        if (IsgameMusicPlaying) {
            StopMusicStream(PlayingMusic);
            startMenuMusicPlaying = false;
        }
        if (!startMenuMusicPlaying) {
            PlayMusicStream(startMenuMusic);
            SetMusicVolume(startMenuMusic, 0.7f);
            startMenuMusicPlaying = true;
        }
       
        player.SetHealth(100);
        player.SetCoins(0);
        player.SetSpeed(200);
		player.SetCooldown(0.5f);
        break;

    case GameState::Menu: {
        if (startMenuMusicPlaying) {
            StopMusicStream(startMenuMusic);
            startMenuMusicPlaying = false;
        }
        coins = player.GetCoins();

        if (IsKeyPressed(KEY_ENTER)) state = GameState::Playing;

        camera.target = player.GetPosition();
        break;
    }
    
    case GameState::Playing: {
        if (startMenuMusicPlaying) {
            StopMusicStream(startMenuMusic);
            startMenuMusicPlaying = false;
        }
        if (!IsgameMusicPlaying) {
            PlayMusicStream(PlayingMusic);
            SetMusicVolume(PlayingMusic, 0.02f);
            IsgameMusicPlaying = true;
        }
        Vector2 prevPos = player.GetPosition();

        player.Update(dt, camera);

        player.Shoot(bullets);

        for (auto& b : bullets) b.Update(dt);
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](Bullet& b) { return !b.active; }), bullets.end());

        HandleCollisions(prevPos);

        Vector2 target = player.GetPosition();
        camera.target.x += (target.x - camera.target.x) * 0.1f;
        camera.target.y += (target.y - camera.target.y) * 0.1f;

        
        UpdateEnemies(dt);  

        if (enemies.empty()) {
            SpawnWave();
        }

		HandleEnemyCollisions();

		if (player.GetHealth() <= 0) {
            
			state  = GameState::GameOver;
		    
		}

        if (IsKeyPressed(KEY_ESCAPE)) {
            state= GameState::PauseMenu;
        }

        if (IsKeyPressed(KEY_ONE) && playerGuns[0]) player.SetGun(GunType::pistol);
        if (IsKeyPressed(KEY_TWO) && playerGuns[1]) player.SetGun(GunType::shotgun);
        if (IsKeyPressed(KEY_THREE) && playerGuns[2]) player.SetGun(GunType::rifle);
        if (IsKeyPressed(KEY_FOUR) && playerGuns[3]) player.SetGun(GunType::sniper);

        break;
    }
    
    case GameState::GameOver: {

        if (IsgameMusicPlaying) {
            StopMusicStream(PlayingMusic);
            startMenuMusicPlaying = false;
        }
		if (IsKeyPressed(KEY_ENTER)) {
			state = GameState::StartMenu;
		}
        if (IsKeyPressed(KEY_ESCAPE)) {
            state = GameState::PauseMenu;
        }
		DrawText("GAME OVER! Press ENTER to return to Main Menu", 200, 280, 30, RED);
        break;
    }
    }
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    switch (state) {

    case GameState::StartMenu:{
        DrawTexturePro(
            StartMenu_bg,
            { 0, 0, (float)StartMenu_bg.width, (float)StartMenu_bg.height },
            { 0, 0, (float)screenWidth, (float)screenHeight },
            { 0, 0 },
            0.0f,
            WHITE
        );

        if (utils::Button({ (float)1215, ((float)GetScreenHeight() / 2)+115, 250, 100 }, "",25)) {
            state = GameState::Playing;
        }
        if (utils::Button({ (float)1215 , ((float)GetScreenHeight() / 2)+310, 250, 100 }, "",25)) {
            exitRequest = true;
        }
        break;
	}
    
    case GameState::Menu: {
        DrawTexturePro(
            ShopMenu,
            { 0, 0, (float)background.width, (float)background.height },
            { 0, 0, (float)background.width, (float)background.height },
            { 0, 0 },
            0.0f,
            WHITE
        );
        
        //Gun Buttons
        {
            // Gun 0 (Pistol)
            if (playerGuns[0] == 0 && utils::Button({ 130, 428, 320, 550 }, "", 0) && player.GetCoins() >= 0) {
                player.SpendCoins(0);
                playerGuns[0] = 1;          
                player.SetGun(GunType::pistol);    
            }

            // Gun 1 (Shotgun)
            if (playerGuns[1] == 0 && utils::Button({ 590, 428, 320, 550 }, "", 0) && player.GetCoins() >= 100) {
                player.SpendCoins(100);
                playerGuns[1] = 1;
                player.SetGun(GunType::shotgun);
            }

            // Gun 2 (Rifle)
            if (playerGuns[2] == 0 && utils::Button({ 1050, 428, 320, 550 }, "", 0) && player.GetCoins() >= 300) {
                player.SpendCoins(300);
                playerGuns[2] = 1;
                player.SetGun(GunType::rifle);
            }

            // Gun 3 (Sniper)
            if (playerGuns[3] == 0 && utils::Button({ 1490, 428, 320, 550 }, "", 0) && player.GetCoins() >= 400) {
                player.SpendCoins(400);
                playerGuns[3] = 1;
                player.SetGun(GunType::sniper);
            }
        }
        
        for (int i=0; i<=3; i++)
        {
            DrawTexturePro(
                player.GetGunTexture(i),
                { 0, 0, (float)player.GetGunTexture(i).width, (float)player.GetGunTexture(i).height },  // source
                { (float)200+460*i, 800, (float)player.GetGunTexture(i).width * 9, (float)player.GetGunTexture(i).height * 9 }, // destination
                { (player.GetGunTexture(i).width / 2.0f), (player.GetGunTexture(i).height / 2.0f) }, // origin (center)
                -70.0f,
                WHITE
            );
        }

		DrawText(TextFormat("%d", player.GetCoins()), 1700, 220, 40, WHITE);
        break;
    }

    case GameState::Playing:
    {
        BeginMode2D(camera);
        DrawTexturePro(
            background,
            { 0, 0, (float)background.width, (float)background.height },
            { 0, 0, (float)background.width, (float)background.height },
            { 0, 0 },
            0.0f,
            WHITE
        );
 
        DrawTexturePro(
            Shop,
            { 0, 0, (float)Shop.width, (float)Shop.height },          // source
            { 530, 130, (float)Shop.width, (float)Shop.height },      // destination (same size)
            { 0, 0 },                                                  // origin (top-left)
            0.0f,
            WHITE
        );

        player.Draw();
        for (auto& b : bullets) b.Draw();

        for (auto& e : enemies)
            e.Draw();
        

        Rectangle shopRect = {
          530.0f,             // x
          130.0f,             // y
          (float)Shop.width,  // width
          (float)Shop.height  // height
        };
        Vector2 shopCenter = { shopRect.x + shopRect.width / 2, shopRect.y + shopRect.height / 2 };
        float shopRadius = 150.0f;

        if (CheckCollisionCircles(player.GetCenter(), player.GetRadius(), shopCenter, shopRadius)) {
            DrawText("Press E to Enter Shop", shopCenter.x - 60, shopCenter.y+50 , 5, YELLOW);

            if (IsKeyPressed(KEY_E)) {
                state = GameState::Menu; // or GameState::Shop if you have a separate shop state
            }
        }

        EndMode2D();
        int currentHealth = player.GetHealth();

        
        int index = (currentHealth * 4) / 100;

       
        if (index < 1) index = 1;
        if (index > 3) index = 3;

        int seconds = (int)player.GetSurvivalTime();
        int minutes = seconds / 60;
        seconds %= 60;

        DrawTexturePro(
            player.GetCurrentGunTexture(),
            { 0, 0, (float)player.GetCurrentGunTexture().width, (float)player.GetCurrentGunTexture().height },  // source
            { 50, 1000, (float)player.GetCurrentGunTexture().width*4, (float)player.GetCurrentGunTexture().height*4 }, // destination
            { (player.GetCurrentGunTexture().width / 2.0f), (player.GetCurrentGunTexture().height / 2.0f)  }, // origin (center)
            -90.0f, 
            WHITE
        );
       
        DrawText(TextFormat("Survived: %02d:%02d", minutes, seconds), 870, 50, 25, WHITE);
        DrawTextureEx(healthBars[index], { 20, 20 }, 0.0f, 0.40, WHITE);

        break;
    }

    case GameState::GameOver: {
        DrawTexturePro(
            GameOverTexture,
            { 0, 0, (float)StartMenu_bg.width, (float)StartMenu_bg.height },
            { 0, 0, (float)screenWidth, (float)screenHeight },
            { 0, 0 },
            0.0f,
            WHITE
        );

         seconds = (int)player.GetSurvivalTime();
         minutes = seconds / 60;
         seconds %= 60;
         score = ((int)player.GetSurvivalTime() * coins);
         if (score < 0) score = 0;

        DrawText(TextFormat("%d", score), 1480, 420, 40, WHITE);
        DrawText(TextFormat("%02d:%02d", minutes, seconds), 1540, 560, 40, WHITE);
        DrawText(TextFormat("%d",player.GetCoins()), 1540, 680, 40, WHITE);
        
    
        break;
    }
    
	case GameState::PauseMenu:
        DrawTexturePro(
            PauseMenu,
            { 0, 0, (float)PauseMenu.width, (float)PauseMenu.height },
            { 0, 0, (float)screenWidth, (float)screenHeight },
            { 0, 0 },
            0.0f,
            WHITE
        );

        if (utils::Button({ (float)1225, ((float)GetScreenHeight() / 2) + 115, 500, 120 }, "", 25)) {
            state = GameState::Playing;
        }
        if (utils::Button({ (float)1225 , ((float)GetScreenHeight() / 2) + 310, 250, 100 }, "", 25)) {
            exitRequest = true;
        }
		break;
    }
    EndDrawing();
}

void Game::HandleCollisions(Vector2 prevPos) {
   
    Vector2 pos = player.GetPosition();
    float radius = player.GetRadius();

    for (const auto& wall : walls) {
        float closestX = fmaxf(wall.x, fminf(pos.x, wall.x + wall.width));
        float closestY = fmaxf(wall.y, fminf(pos.y, wall.y + wall.height));

        float dx = pos.x - closestX;
        float dy = pos.y - closestY;

        if ((dx * dx + dy * dy) < (radius * radius)) {
            player.SetPosition(prevPos);
            break;
        }
    }
}

void Game::SpawnWave() {
    enemies.clear();

    float camLeft = camera.target.x - camera.offset.x / camera.zoom;
    float camRight = camera.target.x + camera.offset.x / camera.zoom;
    float camTop = camera.target.y - camera.offset.y / camera.zoom;
    float camBottom = camera.target.y + camera.offset.y / camera.zoom;

    Vector2 playerPos = player.GetCenter();
    float safeRadius = 400.0f;

    for (int i = 0; i < enemiesToSpawn; i++) {

        Vector2 pos;
        do {
            pos = {
                (float)GetRandomValue((int)camLeft, (int)camRight),
                (float)GetRandomValue((int)camTop,  (int)camBottom)
            };

            float dx = pos.x - playerPos.x;
            float dy = pos.y - playerPos.y;

            // If inside safe zone, retry
            if ((dx * dx + dy * dy) >= safeRadius * safeRadius)
                break;

        } while (true);

        enemies.emplace_back(pos, 18.0f, 100);
    }

    enemiesToSpawn = (int)(enemiesToSpawn * 1.2f);
    wave++;
}

void Game::UpdateEnemies(float dt) {
    int gridCellSize = 32; 

    
    for (auto& e : enemies) {
        if (e.IsAlive())
            e.Update(dt, player.GetCenter());
    }

    for (auto& b : bullets) {
        if (!b.active) continue;

        for (auto& e : enemies) {
            if (!e.IsAlive()) continue;

            float dx = b.position.x - e.GetCenter().x;
            float dy = b.position.y - e.GetCenter().y;
            float distSqr = dx * dx + dy * dy;
            float rSum = b.GetRadius() + e.GetRadius();
            if (distSqr <= rSum * rSum) {
                b.active = false;
                e.TakeDamage(player.GetGunDamage());

                if (!e.IsAlive() && GetRandomValue(0, 100) < 60)
                    player.AddCoins(10); 
            }
        }
    }

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& e) { return !e.IsAlive(); }),
        enemies.end()
    );
}

void Game::HandleEnemyCollisions() {
    Vector2 pc = player.GetCenter();
    float pr = player.GetRadius();

    
    for (auto& e : enemies) {
        if (!e.IsAlive()) continue;

        
        if (CheckCollisionCircles(pc, pr, e.GetCenter(), e.GetRadius())) {
            if (player.damageCooldown <= 0.0f) {
                player.SetHealth(player.GetHealth() - 10);
                player.damageCooldown = 0.5f;
            }
        }
    }

    for (size_t i = 0; i < enemies.size(); i++) {
        Enemy& e1 = enemies[i];
        if (!e1.IsAlive()) continue;

        for (size_t j = i + 1; j < enemies.size(); j++) {
            Enemy& e2 = enemies[j];
            if (!e2.IsAlive()) continue;

            Vector2 dir = Vector2Subtract(e1.GetCenter(), e2.GetCenter());
            float dist = Vector2Length(dir);
            float minDist = e1.GetRadius() + e2.GetRadius();

            if (dist < minDist && dist > 0.0f) {
               
                Vector2 push = Vector2Scale(Vector2Normalize(dir), (minDist - dist) * 0.5f);
                e1.SetPosition(Vector2Add(e1.GetCenter(), push));
                e2.SetPosition(Vector2Subtract(e2.GetCenter(), push));
            }
        }
    }
}

bool Game::Getexitrequest() {
	return exitRequest;
}