#include "raylib.h"
#include "Game.h"

int main() {

    const int screenWidth = 1920;
    const int screenHeight = 1080;
    InitAudioDevice();
    InitWindow(screenWidth, screenHeight, "Top-Down Shooter");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);


    Game game(screenWidth, screenHeight);


    while (!WindowShouldClose()) {
        game.Update();
        game.Draw();

        if (game.Getexitrequest())
        break;

    }
   
    
    CloseWindow();
    return 0;
}
