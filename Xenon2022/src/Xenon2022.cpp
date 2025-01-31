#include "Engine.h"
#include <random>
#include <cmath>
#include <algorithm>
#include <iomanip>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#undef main

GameEngine::Engine engine;
float globalRotation = 0.0f;

//To use this fuction we just call *GetGlobalRotation() and get the value of the global rotation
float* GetGlobalRotation() {
	return &globalRotation;
}

template <typename T>
void removeNullPointers(std::vector<T>& vec) {
	vec.erase(std::remove_if(vec.begin(), vec.end(), [](T ptr) { return ptr == nullptr; }), vec.end());
}

float getRandomFloat(float min, float max) {
	static std::default_random_engine engine{ std::random_device{}() };
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(engine);
}

int getRandomInt(int min, int max) {
	static std::default_random_engine engine{ std::random_device{}() };
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(engine);
}

class MyLevel : public GameLevel
{
public:
	MyLevel(int i)
	{
		SetSortingLayerSize(i);
	}

	void AddScore(int i)
	{
		myScore += i;
	}

	int GetScore()
	{
		return myScore;
	}

private:
	int myScore = 10;


};

class UIScore : public UIText
{
public:
	UIScore(Font* font, std::string text, float posX = 0.f, float posY = 0.f, float sizeX = 1.f, float sizeY = 1.f)
		: UIText(font, text, posX, posY, sizeX, sizeY)
	{
	}

	void OnUpdate() override 
	{
		std::string aux = std::to_string(dynamic_cast<MyLevel*>(engine.getLevel())->GetScore());
		aux = std::string(10 - aux.length(), '0') + aux;
		std::string aux2 = "Score: \n";
		myText = aux2 + aux;
	}
};

class backgroundAssets : public LevelBackground
{
public:

	backgroundAssets(std::string filepath, float sizeX, float sizeY, float posX, float posY, bool tile, int rows, int columns, int numTilesX, int numTilesY, std::vector<int> tileIDs)
		: LevelBackground(filepath, sizeX, sizeY, posX, posY, tile, rows, columns, numTilesX, numTilesY, tileIDs)
	{}

	float moveSpeed = 50.f;
	float initialYPos;

	void OnStart() override
	{
		initialYPos = scrollRect.h;
	}

	void OnUpdate() override 
	{
		scrollRect.h -= moveSpeed * engine.deltaTime;

		if (scrollRect.h < -500.f) 
		{
			resetPosition();
		}
	}

	virtual void resetPosition()
	{
		scrollRect.h = initialYPos;
	}
};

class shiftingBackground : public backgroundAssets
{
public:
	shiftingBackground(std::string filepath, float sizeX, float sizeY, float posX, float posY, bool tile, int rows, int columns, int numTilesX, int numTilesY, std::vector<int> tileIDs)
		: backgroundAssets(filepath, sizeX, sizeY, posX, posY, tile, rows, columns, numTilesX, numTilesY, tileIDs)
	{}
	float baseMoveSpeed = moveSpeed;
	void resetPosition() override 
	{
		backgroundAssets::resetPosition();
		scrollRect.w = getRandomFloat(-280.f, 280.f);
		moveSpeed = baseMoveSpeed;
		moveSpeed += getRandomFloat(-10.f, 10.f);

	}
};

class PlayerLife : public LevelBackground
{
public:
	PlayerLife(std::string filepath, float sizeX, float sizeY, float posX, float posY, bool tile, int rows = 1, int columns =  1, int numTilesX = 3, int numTilesY = 1, std::vector<int> tileIDs = {0,0,0,0,0})
		: LevelBackground(filepath, sizeX, sizeY, posX, posY, tile, rows, columns, numTilesX, numTilesY, tileIDs)
	{
	}

	void UpdateNumberOfLives(int i)
	{
		numTiles.x = i;
	}
	
};

class PlayerHealth : public LevelBackground
{
public:
	PlayerHealth(std::string filepath, float sizeX, float sizeY, float posX, float posY, bool tile, int rows = 1, int columns = 1, int numTilesX = 3, int numTilesY = 1, std::vector<int> tileIDs = { 0,0,0,0,0 })
		: LevelBackground(filepath, sizeX, sizeY, posX, posY, tile, rows, columns, numTilesX, numTilesY, tileIDs)
	{
	}
	float maxTiles;

	void OnStart() override 
	{
		maxTiles = numTiles.x;
	}

	void UpdateHPBar(float healthPercent)
	{
		if (healthPercent > 50.f)
		{
			SetColor(0.f, 1.f, 0.f, 1.f);
		}
		else if (healthPercent > 25.f)
		{
			SetColor(1.f, 1.f, 0.f, 1.f);
		}
		else 
		{
			SetColor(1.f, 0.f, 0.f, 1.f);
		}
		numTiles.x = (maxTiles * healthPercent) / 100.f;
	}

};

class powerUpMissile : public GameObject {
public:
	powerUpMissile(bool visibility = true, bool isBullet = true, bool hasSense = true)
		: GameObject(visibility, isBullet, hasSense) {
	}

	float moveSpeed = 30.0f;


	void OnStart() override {
		int textureDimentions[2] = { 4,2 };
		SetSortingLayer(6);
		animation = new Animation("resources/graphics/PUWeapon.bmp", 0.1f, textureDimentions, true, {});
		objectGroup = "powerUpMissile";
		collisionBoxSize.w = 32.0f;
		collisionBoxSize.h = 32.0f;

		rotation = *GetGlobalRotation();

	}

	void OnUpdate() override {
		position.y -= moveSpeed * engine.deltaTime;

		if (position.y < -280) {
			Destroy();
		}
	}

};

class powerUpHeal : public GameObject {
public:
	powerUpHeal(bool visibility = true, bool isBullet = true, bool hasSense = true)
		: GameObject(visibility, isBullet, hasSense) {
	}

	float moveSpeed = 30.0f;


	void OnStart() override {
		int textureDimentions[2] = { 4,2 };
		SetSortingLayer(6);
		animation = new Animation("resources/graphics/PUShield.bmp", 0.1f, textureDimentions, true, {});
		objectGroup = "powerUpHeal";

	}

	void OnUpdate() override {
		position.y -= moveSpeed * engine.deltaTime;

		if (position.y < -280) {
			Destroy();
		}
	}

};

class powerUpCompanion : public GameObject {
public:
	powerUpCompanion(bool visibility = true, bool isBullet = true, bool hasSense = true)
		: GameObject(visibility, isBullet, hasSense) {
	}
	float moveSpeed = 30.0f;


	void OnStart() override {
		int textureDimentions[2] = { 4,5 };
		SetSortingLayer(6);
		animation = new Animation("resources/graphics/clone.bmp", 0.1f, textureDimentions, true, { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15});
		objectGroup = "powerUpCompanion";

	}

	void OnUpdate() override {

		position.y -= moveSpeed * engine.deltaTime;

		if (position.y < -280) {
			Destroy();
		}
	}

};

class Enemy : public GameObject {
public:
	Enemy(bool visibility = true, bool isBullet = false, bool hasSense = false)
		: GameObject(visibility, isBullet, hasSense) {
	}
	int scoreValue = 10;

public:
	int healthPoints = 1;
	float dropChance = 10.f;
	void showDamageFeedback() {
		modulate.r = 255;
		modulate.g = 0;
		modulate.b = 0;
	}

	void hideDamageFeedback() {
		modulate.r = 255;
		modulate.g = 255;
		modulate.b = 255;
	}

	void checkDamageFeedback() {
		if (damageFeedbackTimer > 0) {
			damageFeedbackTimer -= damageFeedbackDuration * damageFeedbackSpeed * engine.deltaTime;

			if (int(damageFeedbackTimer) % 2 == 0) {
				showDamageFeedback();
			}
			else {
				hideDamageFeedback();
			}

		}
		else
		{
			hideDamageFeedback();
			damageFeedbackTimer = 0;
		}
	}

	void CreatePowerUp(GameObject* powerUp, float posX, float posY)
	{
		powerUp->position.x = posX;
		powerUp->position.y = posY;
		engine.getLevel()->addObject(powerUp);

	}

	void TakeDamage(int paramFirePower) {
		healthPoints -= paramFirePower;

		if (healthPoints <= 0) 
		{
			if (getRandomFloat(0.f, 100.f) >= (100.f - dropChance))
			{
				switch (getRandomInt(0, 3))
				{
				case 0:

					CreatePowerUp(new powerUpHeal(true, true, true), position.x, position.y);

					break;
				case 1:

					CreatePowerUp(new powerUpMissile(true, true, true), position.x, position.y);

					break;
				case 2:

					CreatePowerUp(new powerUpCompanion(true, true, true), position.x, position.y);

					break;
				default:
					break;
				}
			}
			Destroy();
			dynamic_cast<MyLevel*>(engine.getLevel())->AddScore(scoreValue);
		}
		else {

			showDamageFeedback();
			damageFeedbackTimer = damageFeedbackDuration;

		}
	}
private:
	float damageFeedbackTimer = 0;
	float damageFeedbackDuration = 5;
	float damageFeedbackSpeed = 2;

};

class explosion : public GameObject {
public:
	explosion(bool visibility = true, bool isBullet = false, bool hasSense = false)
		: GameObject(visibility, isBullet, hasSense) {}

	void OnStart() override {

		int textureDimentions[2] = { 5,2 };
		SetSortingLayer(5);

		animation = new Animation("resources/graphics/explode64.bmp", 0.1f, textureDimentions, false, {});
	}

	void OnAnimationFinish() override {
		Destroy();
	}

};

class missile : public GameObject {
public:

	missile(bool visibility = true, bool isBullet = false, bool hasSense = true)
		: GameObject(visibility, isBullet, hasSense) {
	}

	float moveSpeed = 250.0f;

	int firePower = 0;
	int missileDamage = 1;

	void OnStart() override {
		int textureDimentions[2] = { 2,3 };
		SetSortingLayer(5);
		switch (firePower) {
		case 0:
			animation = new Animation("resources/graphics/missile.bmp", 0.1f, textureDimentions, true, { 0 ,1});
			break;
		case 1:
			animation = new Animation("resources/graphics/missile.bmp", 0.1f, textureDimentions, true, {2,3});
			break;
		case 2:
			animation = new Animation("resources/graphics/missile.bmp", 0.1f, textureDimentions, true, { 4,5 });
			break;
		default:
			animation = new Animation("resources/graphics/missile.bmp", 0.1f, textureDimentions, true, {0, 1});
			break;
		}

		collisionBoxSize.w = collisionBoxSize.h = 16.0f;

		objectGroup = "bullet";

		rotation = *GetGlobalRotation();
	}

	int getMissileDamage() {
		int damage = 1;

		switch (firePower) {
		case 0:
			damage = 1;
			break;
		case 1:
			damage = 2;
			break;
		case 2:
			damage = 4;
			break;
		}
		return damage;
	}

	void OnUpdate() override {
		position.y += moveSpeed * engine.deltaTime;
		
		if (position.y > 250) {
			Destroy();
		}
	}
};

class rusher : public Enemy {
public:

	rusher(bool visibility = true, bool isBullet = false, bool hasSense = true)
		: Enemy(visibility, isBullet, hasSense) {
	}

	float moveSpeed = 150.0f;
	void OnStart() override {
		healthPoints = 2;
		scoreValue = 5;
		int textureDimentions[2] = { 4,6 };

		animation = new Animation("resources/graphics/rusher.bmp", 0.05f, textureDimentions, true, {});
		objectGroup = "enemy";
		collisionBoxSize.w = 48.0f;
		collisionBoxSize.h = 32.0f;

		rotation = *GetGlobalRotation();
	}

	void OnUpdate() override {
		position.y -= moveSpeed * engine.deltaTime;

		if (position.y < -300) {
			Destroy();
		}

		checkDamageFeedback();
	}

	void OnCollideEnter(GameObject& contact) override {
		if (contact.objectGroup == "bullet") {

			explosion* boom = new explosion();
			boom->position.x = position.x;
			boom->position.y = position.y;
			engine.getLevel()->addObject(boom);

			if (missile* missileContact = dynamic_cast<missile*>(&contact)) {

				int missileFirePower = missileContact->getMissileDamage();
				TakeDamage(missileFirePower);
			}

			contact.Destroy();
		}
	}

};

class enemyProjectile : public GameObject {
public:
	enemyProjectile(bool visibility = true, bool isBullet = true, bool hasSense = true)
		: GameObject(visibility, isBullet, hasSense) {
	}

	float moveSpeed = 250.0f;

	void OnStart() override {
		int textureDimentions[2] = { 8,1 };
		SetSortingLayer(5);
		animation = new Animation("resources/graphics/EnWeap6.bmp", 0.1f, textureDimentions, true, {});
		objectGroup = "enemyBullet";

		collisionBoxSize.w = collisionBoxSize.h = 16.0f;
	}

	void OnUpdate() override {
		position.y -= moveSpeed * engine.deltaTime;

		if (position.y < -280.f) {
			Destroy();
		}
	}

};

class loner : public Enemy {
public:

	loner(bool visibility = true, bool isBullet = false, bool hasSense = true)
		: Enemy(visibility, isBullet, hasSense) {
	}

	float moveSpeed = 70.0f;
	float time = 0.0f;
	float timeCooldown = 2.0f;

	void OnStart() override {

		scoreValue = 7;

		healthPoints = 3;

		int textureDimentions[2] = { 4,4 };

		animation = new Animation("resources/graphics/LonerA.bmp", 0.05f, textureDimentions, true, {});
		objectGroup = "enemy";

		collisionBoxSize.w = collisionBoxSize.h = 64.0f;
		rotation = *GetGlobalRotation();
	}

	void OnCollideEnter(GameObject& contact) override {
		if (contact.objectGroup == "bullet") {

			explosion* boom = new explosion(true, false, false);

			boom->position.x = position.x;
			boom->position.y = position.y;
			engine.getLevel()->addObject(boom);

			if (missile* missileContact = dynamic_cast<missile*>(&contact)) {

				int missileFirePower = missileContact->getMissileDamage();

				TakeDamage(missileFirePower);
			}
			contact.Destroy();
		}
	}
	void OnUpdate() override {
		time += 1 * engine.deltaTime;

		if (time > timeCooldown) {
			enemyProjectile* enemyProj = new enemyProjectile();
			enemyProj->position.x = position.x - 10;
			enemyProj->position.y = position.y - 35;
			engine.getLevel()->addObject(enemyProj);
			time = 0;
		}

		position.x += moveSpeed * engine.deltaTime;

		checkDamageFeedback();

		if (position.x > 360) {
			Destroy();
		}

	}

};

class metalAsteroid : public GameObject {
public:

	metalAsteroid(bool visibility = true, bool isBullet = true, bool hasSense = true)
		: GameObject(visibility, isBullet, hasSense) {
	}
	std::vector<int> asteroidTypes = {32, 64, 96};
	float moveSpeed = 60.0f;
	int asteroidSize = asteroidTypes[getRandomInt(0, 3)];

	void OnStart() override {

		int textureDimentions[2] = { 8,2 };
		SetSortingLayer(6);

		objectGroup = "enemy";

		switch (asteroidSize) {
		case 64:
			textureDimentions[0] = 8;
			textureDimentions[1] = 3;
			animation = new Animation("resources/graphics/MAster64.bmp", 0.1f, textureDimentions, true, {});
			break;
		case 96:
			textureDimentions[0] = 5;
			textureDimentions[1] = 5;
			animation = new Animation("resources/graphics/MAster96.bmp", 0.1f, textureDimentions, true, {});
			break;
		default:
			textureDimentions[0] = 8;
			textureDimentions[1] = 2;
			animation = new Animation("resources/graphics/MAster32.bmp", 0.1f, textureDimentions, true, {});
		}

		collisionBoxSize.w = collisionBoxSize.h = asteroidSize;
		rotation = globalRotation;
	}

	void OnUpdate() override {
		position.y -= moveSpeed * engine.deltaTime;

		if (position.y < -300) {
			Destroy();

		}

	}

	void OnCollideEnter(GameObject& contact) override {

		if (contact.objectGroup == "bullet") {
			contact.Destroy();

		}
	}


};

class stoneAsteroid : public Enemy {
public:
	stoneAsteroid(bool visibility = true, bool isBullet = false, bool hasSense = true)
		: Enemy(visibility, isBullet, hasSense) {
	}
	struct
	{
		float x = 0.0f;
		float y = 32.0f;
	}moveSpeed;

	int asteroidSize;

	void OnStart() override {

		int textureDimentions[2];
		SetSortingLayer(6);
		objectGroup = "enemy";

		switch (asteroidSize) {
		case 64:
			healthPoints = 3;
			scoreValue = 20;
			textureDimentions[0] = 8;
			textureDimentions[1] = 3;
			animation = new Animation("resources/graphics/SAster64.bmp", 0.1f, textureDimentions, true, {});
			break;
		case 96:
			healthPoints = 6;
			scoreValue = 15;
			textureDimentions[0] = 5;
			textureDimentions[1] = 5;
			animation = new Animation("resources/graphics/SAster96.bmp", 0.1f, textureDimentions, true, {});
			break;
		default:
			healthPoints = 1;
			scoreValue = 10;
			textureDimentions[0] = 8;
			textureDimentions[1] = 2;
			animation = new Animation("resources/graphics/SAster32.bmp", 0.1f, textureDimentions, true, {});
		}

		collisionBoxSize.w = collisionBoxSize.h = asteroidSize;
		rotation = globalRotation;
	}

	void createAsteroid(float posX, float posY, float size, float speedX, float speedY) {
		stoneAsteroid* asteroid = new stoneAsteroid();
		asteroid->position.x = posX;
		asteroid->position.y = posY;
		asteroid->asteroidSize = size;
		asteroid->moveSpeed.x = speedX;
		asteroid->moveSpeed.y = speedY;
		engine.getLevel()->addObject(asteroid);
	}

	void OnDestroyed() override {


		switch (asteroidSize) {
		case 64:

			createAsteroid(position.x, position.y, 32, 32.0f, 32.0f);
			createAsteroid(position.x, position.y, 32, -32.0f, 32.0f);

			break;
		case 96:

			createAsteroid(position.x, position.y, 64, 32.0f, 32.0f);
			createAsteroid(position.x, position.y, 64, -32.0f, 32.0f);
			createAsteroid(position.x, position.y, 64, 0.0f, 32.0f);

			break;
		default:

			break;
		}

	}

	void OnUpdate() override {

		position.x += moveSpeed.x * engine.deltaTime;
		position.y -= moveSpeed.y * engine.deltaTime;

		if (position.x > 350 || position.x < -350) {
			Destroy();
		}

		if (position.y < -300) {
			Destroy();
		}

		checkDamageFeedback();

	}

	void OnCollideEnter(GameObject& contact) override {

		if (contact.objectGroup == "bullet") {

			if (missile* missileContact = dynamic_cast<missile*>(&contact)) {

				int missileFirePower = missileContact->getMissileDamage();
				contact.Destroy();
				TakeDamage(missileFirePower);
			}

			explosion* boom = new explosion();
			boom->position.x = position.x;
			boom->position.y = position.y;
			engine.getLevel()->addObject(boom);

		}
	}
};

class drone : public Enemy {
public:
	drone(bool visibility = true, bool isBullet = false, bool hasSense = true)
		: Enemy(visibility, isBullet, hasSense) {
	}

	float moveSpeed = 100.0f;
	float phaseOffset;
	int packID = 0;
	float elapsedTime = 0.f;
	float sinValue = 0.f;
	float aux = 0.f;
	void OnStart() override {
		scoreValue = 2;

		healthPoints = 1;

		aux = position.x;

		int textureDimentions[2] = { 8,2 };

		animation = new Animation("resources/graphics/drone.bmp", 0.1f, textureDimentions, true, {});
		objectGroup = "enemy";


	}

	void OnUpdate() override {

		position.y -= moveSpeed * engine.deltaTime;

		elapsedTime += engine.deltaTime;
		sinValue = (sin(4.f * elapsedTime) * 0.6f);
		position.x = aux + (sin(4.f * elapsedTime) * 40.f);

		if (position.y < -280) {
			Destroy();
		}

		checkDamageFeedback();

	}

	void OnCollideEnter(GameObject& contact) override {
		if (contact.objectGroup == "bullet") {

			explosion* boom = new explosion();
			boom->position.x = position.x;
			boom->position.y = position.y;
			engine.getLevel()->addObject(boom);

			if (missile* missileContact = dynamic_cast<missile*>(&contact)) {

				int missileFirePower = missileContact->getMissileDamage();

				TakeDamage(missileFirePower);
			}

			contact.Destroy();

		}
	}

};

class dronePack : public GameObject
{
public:
	dronePack(bool visibility = false, bool isBullet = false, bool hasSense = false)
		: GameObject(visibility, isBullet, hasSense) {
	}

	int myDroneNumber = 10;
	std::vector<drone*> myPeasents;
	float time = 0.f;
	float spawnCooldown = 0.3f;


	void OnUpdate() override
	{

	
		if (myDroneNumber != 0)
		{
				time += 1 * engine.deltaTime;
				if (time > spawnCooldown) 
				{
					drone* peasent = new drone(true, false, true);
					peasent->SetSortingLayer(3);
					float phaseOffset = myPeasents.size() * 0.2f;
					myPeasents.push_back(peasent);
					peasent->position.x = position.x + phaseOffset;
					peasent->position.y = position.y;
					peasent->phaseOffset = phaseOffset;
					engine.getLevel()->addObject(peasent);
					time = 0;
					myDroneNumber--;
				}
		}

		removeNullPointers(myPeasents);

		if (myPeasents.empty() && myDroneNumber == 0)
		{
			Destroy();
		}

	}

};

class metalAsteroidSpawner : public GameObject
{
public:
	metalAsteroidSpawner(bool visibility = false, bool isBullet = false, bool hasSense = false)
		: GameObject(visibility, isBullet, hasSense) {
	}

	float spawnCooldown = 10.0f;
	float time = 0.0f;

	void OnStart() override {
		objectGroup = "MASpwaner";
	}

	void OnUpdate() override {
		time += engine.deltaTime;
		if (time > spawnCooldown) {
			metalAsteroid* entity = new metalAsteroid(true, false, true);

			entity->position.x = getRandomFloat(-290.f, 290.f);
			entity->position.y = 300.0f;
			engine.getLevel()->addObject(entity);
			time = 0;
		}
	}
};

class stoneAsteroidSpawner : public GameObject
{
public:
	stoneAsteroidSpawner(bool visibility = false, bool isBullet = false, bool hasSense = false)
		: GameObject(visibility, isBullet, hasSense) {
	}

	float spawnCooldown = 20.0f;
	float time = 0.0f;
	std::vector<int> asteroidSizes = { 32, 64, 96 };


	void OnStart() override {
		objectGroup = "SASpwaner";
	}

	void OnUpdate() override {
		time += engine.deltaTime;
		if (time > spawnCooldown) {
			stoneAsteroid* entity = new stoneAsteroid(true, false, true);

			entity->asteroidSize = asteroidSizes[getRandomInt(0, 3)];
			entity->position.x = getRandomFloat(-280, 280);
			entity->position.y = 300.f;
			engine.getLevel()->addObject(entity);
			time = 0;
		}
	}
};

class ally : public Pawn {
public:
	ally(bool visibility = true, bool isBullet = false, bool hasSense = false)
		: Pawn(visibility, isBullet, hasSense) {
	}
	float shipHealthMax = 5.f;
	float shipHealth = 5.f;

	bool keyPressed = false;

	int firePower = 0;

	int positionOffset = 0;

	struct
	{
		int x = 0;
		int y = 0;
	}bulletOffset;


	virtual void TakeShipDamage(){
		if (damageCooldown <= 0)
		{
			shipHealth -= 1;
			damageCooldown = damageCooldownDefault;
		}
	}

	virtual void checkDamageCooldown() {
		if (damageCooldown > 0)
		{
			damageCooldown -= 1 * engine.deltaTime;
		}
		else {
			damageCooldown = 0;
		}
	}
	void ShootCheck() {
		if (input.IsGamepadButtonPressed(GamepadButton::A, false)) {
			if (!keyPressed) {
				missile* bullet = new missile(true, true, true);
				bullet->position.x = position.x + bulletOffset.x;
				bullet->position.y = position.y + bulletOffset.y;
				bullet->firePower = firePower;
				engine.getLevel()->addObject(bullet);
				keyPressed = true;
			}
		}
		else {
			keyPressed = false;
		}
	};
protected:
	float damageCooldownDefault = 1;
	float damageCooldown = 0;
};

class companion : public ally {
public:
	companion(bool visibility = true, bool isBullet = false, bool hasSense = false)
		: ally(visibility, isBullet, hasSense) {
	}

	int textureDimentions[2] = { 4,5 };

	void OnStart() override {

		shipHealthMax = 3;
		shipHealth = 3;
		keyPressed = false;
		firePower = 0;

		SetSortingLayer(4);

		bulletOffset.x = 0;
		bulletOffset.y = 9;

		
		animation = new Animation("resources/graphics/clone.bmp", 0.1f, textureDimentions, true, { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15});
		objectGroup = "companion";
		collisionBoxSize.w = collisionBoxSize.h = 32.0f;
		rotation = globalRotation;
	}

	void HealShip() {
		int healAmount = 2;

		shipHealth += healAmount;

		if (shipHealth > shipHealthMax) {
			shipHealth = shipHealthMax;
		}
	}
	void UpgradeFirePower() {
		if (firePower < 2)
		{
			firePower++;

		}
	}

	void OnUpdate() override
	{
		ShootCheck();
		checkDamageCooldown();

		if (shipHealth <= 0) {

			Destroy();
			isInit = false;
			animation = new Animation("resources/graphics/clone.bmp", 0.1f, textureDimentions, false, {16,17,18,19});
		}
	}

	void OnCollideEnter(GameObject& contact) override 
	{
		if (contact.objectGroup == "enemyBullet") {
			explosion* boom = new explosion();
			boom->position.x = position.x;
			boom->position.y = position.y;
			isInit = false;
			animation = new Animation("resources/graphics/clone.bmp", 1.f, textureDimentions, false, {19});
			engine.getLevel()->addObject(boom);
			TakeShipDamage();
			contact.Destroy();
		}

		if (contact.objectGroup == "enemy") {
			isInit = false;
			animation = new Animation("resources/graphics/clone.bmp", 0.1f, textureDimentions, false, {19});
			TakeShipDamage();
		}

		if (contact.objectGroup == "powerUpMissile") {
			UpgradeFirePower();
			contact.Destroy();
		}

		if (contact.objectGroup == "powerUpHeal") {
			HealShip();
			contact.Destroy();
		}
	}
};

class spaceship : public ally {
public:
	spaceship(bool visibility = true, bool isBullet = false, bool hasSense = false)
		: ally(visibility, isBullet, hasSense) {
	}

	int textureDimentions[2] = { 7,3 };

	std::string currentAnimation = "";
	int animationState = 0;

	bool isGameOver = false;

	std::vector<float> companionOffset = {65.f, -65.f};

	bool canTakeDamage = true;
	float damageCooldown = 0;
	bool onAnimation = false;

	float limits[4] = { 290.f, -210.f, -290.f, 210.f };

	int lives = 3;

	std::vector<companion*> myCompanions;

	PlayerLife* playerLifesUI;
	PlayerHealth* playerHealthUI;

	struct 
	{
		float x = 0;
		float y = 0;
	}respawnPosition;

	void OnStart() override {

		shipHealthMax = 10;
		shipHealth = 10;
		keyPressed = false;
		firePower = 0;

		movementSpeed = 200.0f;

		bulletOffset.x = 0;
		bulletOffset.y = 24;

		animationState = 0;
		objectGroup = "player";

		respawnPosition.x = position.x = 0.0f;
		respawnPosition.y = position.y = -100.0f;

		collisionBoxSize.w = collisionBoxSize.h = 64.0f;
		rotation = *GetGlobalRotation();

		std::vector<int> lifeTiles = { 0, 0, 0, 0, 0 };
		playerLifesUI = new PlayerLife("resources/graphics/PULife.bmp", 0.15f, 0.17f, -290.f, -160.f, true, 1, 1, lives, 1, lifeTiles);
		playerLifesUI->SetSortingLayer(9);
		playerLifesUI->UpdateNumberOfLives(lives);
		engine.getLevel()->addBackground(playerLifesUI);

		std::vector<int> healthTiles = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		playerHealthUI = new PlayerHealth("resources/graphics/Blocks.bmp", 0.02f, 0.1f, -305.f, -210.f, true, 64, 16, 20, 1, healthTiles);
		playerHealthUI->SetSortingLayer(9);
		//playerHealthUI->UpdateHPBar(100.f);
		playerHealthUI->SetColor(0.f, 1.f, 0.f, 1.f);
		engine.getLevel()->addBackground(playerHealthUI);
	}

	void OnAnimationFinish() override 
	{
		onAnimation = false;
		
	}

	void UpdateHealthPercent()
	{
		float percentHealth = (shipHealth / shipHealthMax) * 100;
		playerHealthUI->UpdateHPBar(percentHealth);
	}

	void OnUpdate() override {

		if (isGameOver == false)
		{
			if (myCompanions.size() > 0)
			{
				for (int i = 0; i < myCompanions.size(); i++)
				{
					if (myCompanions[i]->shipHealth <= 0)
					{
						myCompanions[i]->Destroy();
						myCompanions.erase(myCompanions.begin() + i);
					}
				}
			}

			ShootCheck();
			checkDamageCooldown();

			float moveX = 0.0f;
			float moveY = 0.0f;

			if (input.IsGamepadButtonPressed(GamepadButton::DPadLeft, false)) {
				moveX = -1.0f;
				animationState = 2;
			}
			else if (input.IsGamepadButtonPressed(GamepadButton::DPadRight, false)) {
				moveX = 1.0f;
				animationState = 1;
			}
			else {
				animationState = 0;
			}
			if (input.IsGamepadButtonPressed(GamepadButton::DPadUp, false)) {
				moveY = 1.0f;
			}
			else if (input.IsGamepadButtonPressed(GamepadButton::DPadDown, false)) {
				moveY = -1.0f;
			}

			// Normalize the movement vector if moving diagonally
			float magnitude = std::sqrt(moveX * moveX + moveY * moveY);
			if (magnitude > 0.0f) {
				moveX /= magnitude; // Normalize X
				moveY /= magnitude; // Normalize Y
			}

			// Apply the movement
			float auxX = position.x;
			float auxY = position.y;

			auxX += moveX * movementSpeed * engine.deltaTime;
			auxY += moveY * movementSpeed * engine.deltaTime;

			if (auxX < limits[0] && auxX > limits[2])
			{
				position.x += moveX * movementSpeed * engine.deltaTime;
			}

			if (auxY >= limits[1] && auxY <= limits[3])
			{
				position.y += moveY * movementSpeed * engine.deltaTime;
			}
		}

		if (myCompanions.size() > 0)
		{
			for (int i = 0; i < myCompanions.size(); i++)
			{
				myCompanions[i]->position.x = position.x + companionOffset[i];
				myCompanions[i]->position.y = position.y;
			}
		}

		if (animationState == 1 && currentAnimation != "Right" && onAnimation == false)
		{
			currentAnimation = "Right";
			animation = new Animation("resources/graphics/Ship2.bmp", 0.1f, textureDimentions, false, {4,5,6});
		}
		else if (animationState == 2 && currentAnimation != "Left" && onAnimation == false)
		{
			currentAnimation = "Left";
			animation = new Animation("resources/graphics/Ship2.bmp", 0.1f, textureDimentions, false, { 2,1,0 });
		}
		else if (animationState == 0 && currentAnimation != "Idle" && onAnimation == false)
		{
			currentAnimation = "Idle";
			animation = new Animation("resources/graphics/Ship2.bmp", 0.1f, textureDimentions, false, {3});
		}


		if (shipHealth <= 0) {
			if (lives > 0)
			{
				shipHealth = shipHealthMax;
				lives--;
				playerLifesUI->UpdateNumberOfLives(lives);
				UpdateHealthPercent();
				position.x = respawnPosition.x;
				position.y = respawnPosition.y;
			}
			else if (isGameOver == false)
			{
				if (myCompanions.size() > 0)
				{
					for (int i = myCompanions.size() - 1; i >= 0; i--)
					{
						myCompanions[i]->Destroy();
						myCompanions.erase(myCompanions.begin() + i);
					}
				}
				isGameOver = true;
				Destroy();
				playerLifesUI = nullptr;
			}
		}
	}

	void HealShip() {
		int healAmount = 2;

		shipHealth += healAmount;
		UpdateHealthPercent();

		if (shipHealth > shipHealthMax) {
			shipHealth = shipHealthMax;
		}
	}
	void UpgradeFirePower() {
		if (firePower < 2)
		{
			firePower++;

		}
	}
	void RecruitCompanion()
	{
		
		if (myCompanions.size() < 2)
		{
			companion* companion1 = new companion(true, false, true);
			myCompanions.push_back(companion1);
			engine.getLevel()->addObject(companion1);
		}
	}

	void OnCollideEnter(GameObject& contact) override {
		
		if (contact.objectGroup == "enemyBullet") {
			explosion* boom = new explosion();
			boom->position.x = position.x;
			boom->position.y = position.y;

			engine.getLevel()->addObject(boom);
			TakeShipDamage();

			contact.Destroy();
		}

		if (contact.objectGroup == "enemy") {

			TakeShipDamage();

		}
		
		if (contact.objectGroup == "powerUpMissile") {
			UpgradeFirePower();
			contact.Destroy();
		}

		if (contact.objectGroup == "powerUpCompanion") {
			RecruitCompanion();
			contact.Destroy();
		}

		if (contact.objectGroup == "powerUpHeal") {
			HealShip();
			contact.Destroy();
		}
	}

	void checkDamageCooldown() override
	{
		if (damageCooldown > 0)
		{
			damageCooldown -= 1 * engine.deltaTime;
		}
		else {
			damageCooldown = 0;
		}
	}

	void TakeShipDamage() override {

		if (damageCooldown <= 0)
		{
			shipHealth -= 1;
			damageCooldown = damageCooldownDefault;

			UpdateHealthPercent();

			int myFrame = animation->currentFrame;
			onAnimation = true;
			animation = new Animation("resources/graphics/Ship2.bmp", 0.1f, textureDimentions, false,
				{ myFrame + 7 , myFrame, myFrame + 7, myFrame});
		}
	}

};

class droneSpawner : public GameObject
{
public:
	droneSpawner(bool visibility = false, bool isBullet = false, bool hasSense = false)
		: GameObject(visibility, isBullet, hasSense) {
		hasBox2d = false;
	}

	float spawnCooldown = 10.0f;
	float time = 0.0f;

	void OnStart() override {
		objectGroup = "RSpwaner";
	}

	void OnUpdate() override {
		time += engine.deltaTime;
		if (time > spawnCooldown) {
			dronePack* enemy = new dronePack(true, false, true);
			enemy->position.x = getRandomFloat(-240.f, 240.f);
			enemy->position.y = 300.0f;
			engine.getLevel()->addObject(enemy);
			time = 0;
		}
	}
};

class rusherSpawner : public GameObject
{
public:
	rusherSpawner(bool visibility = false, bool isBullet = false, bool hasSense = false)
		: GameObject(visibility, isBullet, hasSense) {
		hasBox2d = false;
	}

	float spawnCooldown = 2.0f;
	float time = 0.0f;

	void OnStart() override {
		objectGroup = "RSpwaner";
	}

	void OnUpdate() override {
		time += 1 * engine.deltaTime;
		if (time > spawnCooldown) {
			rusher* enemy = new rusher(true, false, true);
			enemy->SetSortingLayer(3);
			enemy->position.x = getRandomFloat(-290.f, 290.f);
			enemy->position.y = 300.0f;
			engine.getLevel()->addObject(enemy);
			time = 0;
		}
	}
};

class lonerSpawner : public GameObject
{
public:
	void OnStart() override {
		objectGroup = "LSpwaner";

		position.x = 0;
	}
	lonerSpawner(bool visibility = false, bool isBullet = false, bool hasSense = false)
		: GameObject(visibility, isBullet, hasSense) {
	}
	float spawnCooldown = 4.0f;
	float time = 0.0f;

	void OnUpdate() override {
		time += 1 * engine.deltaTime;
		if (time > spawnCooldown) {
			loner* enemy = new loner(true, false, true);
			enemy->SetSortingLayer(3);
			enemy->position.x = -350.0f;
			enemy->position.y = getRandomFloat(0.f, 205.f);
			engine.getLevel()->addObject(enemy);
			time = 0;
		}
	}
};

class FirstBackground : public LevelBackground
{
public:
	FirstBackground(std::string filepath, float sizeX, float sizeY, float posX, float posY)
		: LevelBackground(filepath, sizeX, sizeY, posX, posY)
	{}
};

int main()
{
	GameWindow gameWindow;
	gameWindow.windowName = "Xenon 2000";
	gameWindow.windowWidth = 640;
	gameWindow.windowHeight = 480;

	MyLevel* level = new MyLevel(10);
	engine.setLevel(level);


	LevelBackground* baseBackground = new LevelBackground("resources/graphics/galaxy2.bmp", 2.f, 2.f, 0.f, 0.f);
	baseBackground->SetSortingLayer(0);

	// Create a tiled background layer
	std::vector<int> tileIDs = { 400,401,402,403,404,416,417,418,419,420,432,433,434,435,436,448,449,450,451,452,464,465,466,467,468,480,481,482,483,484,496,497,498,499,500 };
	shiftingBackground* backgroundAsset = new shiftingBackground("resources/graphics/Blocks.bmp", 0.15f, 0.15f, -150.f, 500.f, true, 64, 16, 5, 7, tileIDs);
	backgroundAsset->SetSortingLayer(1);

	std::vector<int> tileIDs2 = { 768,769,770,771,772,773,784,785,786,787,788,789,800,801,802,803,804,805 };
	shiftingBackground* backgroundAsset2 = new shiftingBackground("resources/graphics/Blocks.bmp", 0.15f, 0.15f, -300.f, 750.f, true, 64, 16, 6, 3, tileIDs2);
	backgroundAsset2->SetSortingLayer(1);
	
	std::vector<int> tileIDs3 = { 591, 636,637,591,651,652,653,654,667,668,669,670,683,684,685,686,699,700,701,702,591,716,717,591,591,732,733,591 };
	shiftingBackground* backgroundAsset3 = new shiftingBackground("resources/graphics/Blocks.bmp", 0.15f, 0.15f, 340.f, 600.f, true, 64, 16, 4, 7, tileIDs3);
	backgroundAsset3->SetSortingLayer(1);
	
	shiftingBackground* backgroundAsset6 = new shiftingBackground("resources/graphics/Blocks.bmp", 0.15f, 0.15f, 100.f, 1100.f, true, 64, 16, 4, 7, tileIDs3);
	backgroundAsset3->SetSortingLayer(1);

	std::vector<int> tileIDs4 = { 304,305,306,307,308,309,310,311,312,313,314,320,321,322,323,324,325,326,327,328,329,330 };
	shiftingBackground* backgroundAsset4 = new shiftingBackground("resources/graphics/Blocks.bmp", 0.15f, 0.15f, 200.f, 850.f, true, 64, 16, 11, 2, tileIDs4);
	backgroundAsset3->SetSortingLayer(2);
	
	shiftingBackground* backgroundAsset5 = new shiftingBackground("resources/graphics/Blocks.bmp", 0.15f, 0.15f, -200.f, 1000.f, true, 64, 16, 11, 2, tileIDs4);
	backgroundAsset3->SetSortingLayer(2);
	
	std::vector<int> tileIDs0 = { 816,817,818,819,820,821,822,823,824,816,816,832,833,834,835,836,837,838,839,840,832,832,848,849,850,851,852,853,854,855,856,848,848,864,865,866,867,868,869,870,871,872,873,874,880,881,882,883,884,885,886,887,888,889,890,896,897,898,899,900,901,902,903,904,905,906,912,913,914,915,916,917,918,919,920,921,922,928,929,930,931,932,933,934,935,936,937,938,944,945,946,947,948,949,950,951,952,954,954,960,961,962,963,964,965,966,967,969,969,969,976,976,976,976,980,981,982,976,976,976,976,992,992,992,992,996,997,998,992,992,992,992 };
	backgroundAssets* backgroundAsset0 = new backgroundAssets("resources/graphics/Blocks.bmp", 0.2f, 0.2f, 0.f, 2500.f, true, 64, 16, 11, 12, tileIDs0);
	backgroundAsset0->SetSortingLayer(0);
	backgroundAsset0->moveSpeed = 10.f;


	level->addBackground(baseBackground);
	level->addBackground(backgroundAsset);
	level->addBackground(backgroundAsset2);
	level->addBackground(backgroundAsset3);
	level->addBackground(backgroundAsset4);
	level->addBackground(backgroundAsset5);
	level->addBackground(backgroundAsset6);
	level->addBackground(backgroundAsset0);

	Font* myFont = new Font("resources/graphics/font16x16.bmp", 8, 12);
	UIScore* myUIScore = new UIScore(myFont, "", -0.9f, 0.85f, 0.05f, 0.05f);


	myUIScore->SetSortingLayer(9);

	engine.getLevel()->addUIText(myUIScore);

	spaceship* ship = new spaceship();
	ship->SetSortingLayer(4);
	engine.getLevel()->addObject(ship);
	
	rusherSpawner* spawner = new rusherSpawner();
	engine.getLevel()->addObject(spawner);

	lonerSpawner* spawner2 = new lonerSpawner();
	engine.getLevel()->addObject(spawner2);

	metalAsteroidSpawner* spawner3 = new metalAsteroidSpawner();
	engine.getLevel()->addObject(spawner3);
	
	stoneAsteroidSpawner* spawner4 = new stoneAsteroidSpawner();
	engine.getLevel()->addObject(spawner4);
	
	droneSpawner* spawner5 = new droneSpawner();
	engine.getLevel()->addObject(spawner5);
	
	powerUpCompanion* p1 = new powerUpCompanion();
	p1->position.x = 0.f;
	p1->position.y = 250;
	engine.getLevel()->addObject(p1);

	engine.Initialize(gameWindow);

}
