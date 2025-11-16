#include "Player.h"
#include "Bullets.h"
#include <cmath> 

Player::Player(Vector2 startPos, float speed, int health)
	: position(startPos), speed(speed), angle(0.0f), health(health), coins(100000)
{
	vampire = LoadTexture(TextFormat("%s/Player/vampire.png", RESOURCES_PATH));
	vampire2 = LoadTexture(TextFormat("%s/Player/vampire2.png", RESOURCES_PATH));

	for (int i = 0; i <= 3; i++) {
		Texture2D tex = LoadTexture(TextFormat("%s/Weapons/Guns/gun_%d.png", RESOURCES_PATH, i));
		guns.push_back(tex);
		Texture2D tex2 = LoadTexture(TextFormat("%s/Weapons/Guns/inverted_gun_%d.png", RESOURCES_PATH, i));
		gunsFlipped.push_back(tex2);
		Sound gunShot = LoadSound(TextFormat("%s/Music/gun_%d.ogg", RESOURCES_PATH, i));
		gunshots.push_back(gunShot);
	}
	for (int i = 0; i < 4; i++)
	{
		SetSoundVolume(gunshots[i], 0.04f);
	}

	currentGun = GunType::pistol;
}

void Player::Update(float dt, const Camera2D& camera) {
	survivalTime += dt;
	Vector2 mouse = GetMousePosition();
	Vector2 worldMouse = GetScreenToWorld2D(mouse, camera);
	angle = atan2f(worldMouse.y - position.y, worldMouse.x - position.x);

	damageCooldown -= dt;
	if (damageCooldown < 0) damageCooldown = 0;
	shootTimer -= dt;
	if (shootTimer < 0) shootTimer = 0;

	// Movement
	Vector2 prevPos = position;
	Move(dt);

	// Determine if walking
	bool isWalking = (position.x != prevPos.x || position.y != prevPos.y);
	if (isWalking) {
		facingRight = (position.x >= prevPos.x);  // true if moving right
		frameTimer += dt;
		if (frameTimer >= frameTime) {
			currentFrame = (currentFrame + 1) % frameCount;
			frameTimer = 0.0f;
		}
	}
	else {
		currentFrame = 0;  // idle frame
	}

	if (health <= 0) Die();
}

void Player::DrawGun() const {

	Texture2D gunTex;
	if (angle > PI / 2 || angle < -PI / 2) {
		gunTex = gunsFlipped[(int)currentGun];
	}
	else {
		gunTex = guns[(int)currentGun];
	}

	float gunDistance = 10.0f;
	Vector2 gunPos = {
		position.x + cosf(angle) * gunDistance,
		position.y + sinf(angle) * gunDistance
	};

	Rectangle srcGun = { 0, 0, (float)gunTex.width, (float)gunTex.height };
	Rectangle destGun = { gunPos.x, gunPos.y - 3, (float)gunTex.width, (float)gunTex.height };
	Vector2 gunOrigin = { 3, (float)(gunTex.height / 2.0f) - 5.0 };

	float drawAngle = angle * RAD2DEG;

	DrawTexturePro(gunTex, srcGun, destGun, gunOrigin, drawAngle, WHITE);
}

void Player::DrawPlayer() const {
	int frameWidthRight = vampire.width / frameCount;   // right-facing texture
	int frameWidthLeft = vampire2.width / frameCount;  // left-facing texture
	int frameHeightRight = vampire.height;
	int frameHeightLeft = vampire2.height;

	Rectangle srcPlayer;
	Rectangle destPlayer;
	Vector2 playerOrigin;

	if (facingRight) {
		srcPlayer = { (float)frameWidthRight * currentFrame, 0, (float)frameWidthRight, (float)frameHeightRight };
		destPlayer = { position.x, position.y + 20, (float)frameWidthRight * (float)1.2, (float)frameHeightRight * (float)1.2 };
		playerOrigin = { frameWidthRight / 2.0f, (float)frameHeightRight }; // bottom-center pivot
		DrawTexturePro(vampire, srcPlayer, destPlayer, playerOrigin, 0.0f, WHITE);
	}
	else {
		srcPlayer = { (float)frameWidthLeft * currentFrame, 0, (float)frameWidthLeft, (float)frameHeightLeft };
		destPlayer = { position.x, position.y + 20, (float)frameWidthLeft * (float)1.2, (float)frameHeightLeft * (float)1.2 };
		playerOrigin = { frameWidthLeft / 2.0f, (float)frameHeightLeft }; // bottom-center pivot
		DrawTexturePro(vampire2, srcPlayer, destPlayer, playerOrigin, 0.0f, WHITE);
	}
}

void Player::Draw() const {
	DrawPlayer();
	DrawGun();
}

void Player::Move(float dt) {
	Vector2 prevPos = position;

	// Move input
	if (IsKeyDown(KEY_W)) position.y -= speed * dt;
	if (IsKeyDown(KEY_S)) position.y += speed * dt;
	if (IsKeyDown(KEY_A)) position.x -= speed * dt;
	if (IsKeyDown(KEY_D)) position.x += speed * dt;
}

void Player::Shoot(std::vector<Bullet>& bullets)
{
	double now = GetTime();
	GunPreset preset = GetGunPreset(currentGun);

	bool mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

	//rifle shoot
	if (currentGun == GunType::rifle)
	{
		if (mouseDown)
		{

			if (!IsSoundPlaying(gunshots[2]))
				PlaySound(gunshots[2]);
		}
		else
		{

			StopSound(gunshots[2]);
		}
	}


	if (mouseDown && (now - lastShotTime) >= preset.fireRate)
	{
		Vector2 spawn = {
			position.x + cosf(angle) * 20.0f,
			position.y + sinf(angle) * 20.0f
		};

		bullets.emplace_back(spawn, angle, preset.bulletSpeed, preset.bulletLife, preset.damage);


		if (currentGun != GunType::rifle)
		{
			int soundIndex = 0;
			switch (currentGun)
			{
			case GunType::pistol:  soundIndex = 0; break;
			case GunType::shotgun: soundIndex = 3; break;
			case GunType::sniper:  soundIndex = 1; break;
			default: break;
			}
			PlaySound(gunshots[soundIndex]);
		}

		lastShotTime = now;
	}
}

void Player::Die() {

	DrawText("You Died!", 350, 280, 30, RED);
}

Vector2 Player::GetPosition() const { return position; }
float Player::GetAngle() const { return angle; }
int Player::GetHealth() const { return health; }
float Player::GetSpeed() const { return speed; }
double Player::GetCooldown() const { return cooldown; }
int Player::GetCoins() const {
	return coins;
}
Vector2 Player::GetCenter() const {
	return position;
}
float Player::GetRadius() const {
	return 6.0f;
}
GunType Player::GetGun() const {
	return currentGun;
}
float Player::GetSurvivalTime() const {
	return survivalTime;
}
Texture2D Player::GetCurrentGunTexture() const {
	return guns[(int)currentGun];
}
Texture2D Player::GetGunTexture(int i) const {
	return guns[i];
}
int Player::GetGunDamage() {
	return GetGunPreset(currentGun).damage;
}


void Player::SetSpeed(float newSpeed) { speed = newSpeed; }
void Player::SetPosition(Vector2 newPos) { position = newPos; }
void Player::SetHealth(int newHealth) { health = newHealth; }
void Player::SetCooldown(int percent) { cooldown *= (1.0 - percent / 100.0); }
void Player::SetGun(GunType newGun) {
	currentGun = newGun;
}
void Player::AddCoins(int amount) {
	coins += amount;
}
void Player::SpendCoins(int amount) {
	coins -= amount;
	if (coins < 0) coins = 0;
}
void Player::SetCoins(int amount) {
	coins = amount;
}
