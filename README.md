PROJECT DESCRIPTION:
Our project is an arcade top-down shooter game that we have developed using the Raylib
library of C++. The player controls a character positioned in an open arena, equipped
initially with a medium-damage gun. Waves of enemies continuously approach the player
from different directions, increasing in and quantity after each completed wave. The core
gameplay revolves around surviving as long as possible, eliminating enemies before they
make contact. Whenever an enemy collides with the player, the player’s health decreases.
The coins you collect after defeating enemies serve as an in-game currency that allows
the player to upgrade their weapon, improving attributes such as damage to the enemy.
Overall, this project has fundamental game development techniques, real-time interaction,
dynamic difficulty leveling, and the practical use of DSA principles like vectors

Project Functionalities:
1. Game Menu System: The game has a menu system that allows players to start the
game, view instructions, and quit the game.
2. Player Movement and Shooting: The player can move their character using WASD
keys and shoot bullets using the mouse.
3. Enemy AI: The enemies have a basic AI that allows them to move towards the player
and shoot bullets.
4. Collision Detection: The game has collision detection for bullets, enemies, and walls.
5. Scorekeeping: The game keeps track of the player's score and displays it on the
screen.
6. Health System: The player has a health system that decreases when they are hit by
enemy bullets.
7. Buying Guns: The game has power-ups that give the player temporary advantages,
such as increased speed or damage.
8. Level System: The game has multiple levels with increasing difficulty.
9. Game Over Screen: The game has a game over screen that displays the player's score
and allows them to restart the game.
10. Sound Effects and Music: The game has sound effects and music that enhance the
gaming experience.
Tools:
Libraries and Frameworks used:
- Raylib: A simple and easy-to-use game development library for C and C++.
- Raymath: A math library for C and C++ that provides vector and matrix operations.
IDE used:
Visual Studio Code (VS Code)
- Compiler: GCC (GNU Compiler Collection)
- 
Tools and Utilities:
- Git: A version control system for managing code changes.
- GitHub: A web-based platform for version control and collaboration.
- CMake: A build system generator for C and C++ projects.
- Make: A build automation tool for C and C++ projects.
Graphics and Rendering:
- Raylib's built-in graphics rendering.
Input and Events:
- Raylib's built-in input handling capabilities.
- Keyboard and mouse input.
Project Structure:
- The project is organized into several source files, each containing a specific class or
module:
- main.cpp: The entry point of the game.
- Game.h/Game.cpp: The game class, responsible for game logic and rendering.
- Player.h/Player.cpp: The player class, responsible for player movement and animation.
- Enemy.h/Enemy.cpp: The enemy class, responsible for enemy AI and movement.
- Bullet.h/Bullet.cpp: The bullet class, responsible for bullet movement and collision
detection.
- Utils.h/Utils.cpp: Utility functions for drawing buttons and stat bar
  
FUNCTIONS DESCRIPTION:
main.cpp
Description: This is the entry point of the game. It initializes the game window, sets up
the game loop, and handles events.
Interactions: Includes Game.h, raylib.h
Functions:
- main(): Initializes the game window, sets up the game loop, and handles events.
- Game game(screenWidth, screenHeight): Creates a new instance of the Game class.
- game.Update(): Calls the Update() function of the Game class.
- game.Draw(): Calls the Draw() function of the Game class.
Game.h and Game.cpp
- Description: This file contains the Game class, which manages the game's state,
updates, and rendering.
- Interactions: Includes Player.h, Bullets.h, Enemy.h, utils.h, raylib.h
- Functions:
- Game(int screenWidth, int screenHeight): Initializes the game state.
- Update(): Updates the game state, including player movement, enemy AI, and collision
detection.
- Draw(): Renders the game state, including the player, enemies, and UI.
- HandleCollisions(): Handles collision detection and response.
- SpawnWave(): Spawns a new wave of enemies.
Player.h and Player.cpp
- Description: This file contains the Player class, which represents the player's character.
- Interactions: Includes Bullets.h, raylib.h
- Functions:
- Player(Vector2 startPos, float speed, int health): Initializes the player's character.
- Update(float dt, const Camera2D& camera): Updates the player's movement and
animation.
- Draw(): Renders the player's character.
- Move(float dt): Moves the player's character based on input.
- Shoot(std::vector<Bullet>& bullets, float speed): Shoots a bullet.
Bullets.h and Bullets.cpp
- Description: This file contains the Bullet class, which represents a bullet fired by the
player or enemy.
- Interactions: Includes raylib.h
- Functions:
- Bullet(Vector2 startPos, float angle, float speed): Initializes a new bullet.
- Update(float dt): Updates the bullet's position and checks for collisions.
- Draw(): Renders the bullet.
Enemy.h and Enemy.cpp
- Description: This file contains the Enemy class, which represents an enemy in the game.
- Interactions: Includes raylib.h, raymath.h
- Functions:
- Enemy(Vector2 pos, float r, int hp): Initializes a new enemy.
- Update(float dt, Vector2 playerPos, const std::vector<Rectangle>& walls, int
gridCellSize): Updates the enemy's AI and movement.
- Draw(): Renders the enemy.
- TakeDamage(int dmg): Applies damage to the enemy.
- IsAlive(): Checks if the enemy is alive.
utils.h and utils.cpp
- Description: This file contains utility functions for the game, such as drawing buttons
and stat bars.
- Interactions: Includes raylib.h, raymath.h
- Functions:
- Button(Rectangle bounds, const std::string& text, int fontSize): Draws a button and
checks for clicks.

Data Structures:
The game primarily uses vectors (vectors) to store dynamic collections of bullets, enemies,
and walls. Vectors are ideal because they provide contiguous memory, fast iteration, and
efficient access by index, which is essential in a real-time game loop where updates and
collision checks must run every frame. Since enemies and bullets are frequently added
and removed, vectors offer efficient amortized insertion at the end and allow cleanup with
remove_if, which avoids expensive memory operations.
The game also uses structs like Vector2 for positions because they are lightweight and
allow efficient mathematical operations for movement and collision detection. Rectangles
(Rectangle) are stored in a vector because collision checks only require sequential
scanning, making a vector the simplest and fastest choice without needing more complex
spatial partitions
