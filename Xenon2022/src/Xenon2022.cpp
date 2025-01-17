#include "Engine.h"
#include <random>
#include <cmath>
#include <algorithm>

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

class powerUpMissile : public GameObject {
public:
	powerUpMissile(bool visibility = true, bool isBullet = true, bool hasSense = true)
		: GameObject(visibility, isBullet, hasSense) {
	}

	float moveSpeed = 30.0f;


	void OnStart() override {
		int textureDimentions[2] = { 4,2 };

		animation = Animation("resources/graphics/PUWeapon.bmp", 0.1f, textureDimentions, true, {});
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

		animation = Animation("resources/graphics/PUShield.bmp", 0.1f, textureDimentions, true, {});
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

		animation = Animation("resources/graphics/clone.bmp", 0.1f, textureDimentions, true, { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15});
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
		engine.getLevel().addObject(powerUp);

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

		animation = Animation("resources/graphics/explode64.bmp", 0.1f, textureDimentions, false, {});
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
		switch (firePower) {
		case 0:
			animation = Animation("resources/graphics/missile.bmp", 0.1f, textureDimentions, true, { 0 ,1});
			std::cout << "Tile Map Size " << animation.tilemapSize.h << std::endl;
			std::cout << "Target Frame " << animation.targetFrame << std::endl;
			std::cout << "Current Frame " << animation.currentFrame << std::endl;
			break;
		case 1:
			animation = Animation("resources/graphics/missile.bmp", 0.1f, textureDimentions, true, {2 ,3});
			break;
		case 2:
			animation = Animation("resources/graphics/missile.bmp", 0.1f, textureDimentions, true, {4, 5});
			break;
		default:
			animation = Animation("resources/graphics/missile.bmp", 0.1f, textureDimentions, true, {0, 1});
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

		int textureDimentions[2] = { 4,6 };

		animation = Animation("resources/graphics/rusher.bmp", 0.05f, textureDimentions, true, {});
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
			engine.getLevel().addObject(boom);

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

		animation = Animation("resources/graphics/EnWeap6.bmp", 0.1f, textureDimentions, true, {});
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

		healthPoints = 3;

		int textureDimentions[2] = { 4,4 };

		animation = Animation("resources/graphics/LonerA.bmp", 0.05f, textureDimentions, true, {});
		objectGroup = "enemy";

		collisionBoxSize.w = collisionBoxSize.h = 64.0f;
		rotation = *GetGlobalRotation();
	}

	void OnCollideEnter(GameObject& contact) override {
		if (contact.objectGroup == "bullet") {

			explosion* boom = new explosion(true, false, false);

			boom->position.x = position.x;
			boom->position.y = position.y;
			engine.getLevel().addObject(boom);

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
			engine.getLevel().addObject(enemyProj);
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

		objectGroup = "enemy";

		switch (asteroidSize) {
		case 64:
			textureDimentions[0] = 8;
			textureDimentions[1] = 3;
			std::cout << "64" << std::endl;
			animation = Animation("resources/graphics/MAster64.bmp", 0.1f, textureDimentions, true, {});
			break;
		case 96:
			textureDimentions[0] = 5;
			textureDimentions[1] = 5;
			std::cout << "96" << std::endl;
			animation = Animation("resources/graphics/MAster96.bmp", 0.1f, textureDimentions, true, {});
			break;
		default:
			textureDimentions[0] = 8;
			textureDimentions[1] = 2;
			std::cout << "32" << std::endl;
			animation = Animation("resources/graphics/MAster32.bmp", 0.1f, textureDimentions, true, {});
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

		objectGroup = "enemy";

		switch (asteroidSize) {
		case 64:
			healthPoints = 3;

			textureDimentions[0] = 8;
			textureDimentions[1] = 3;
			animation = Animation("resources/graphics/SAster64.bmp", 0.1f, textureDimentions, true, {});
			break;
		case 96:
			healthPoints = 6;

			textureDimentions[0] = 5;
			textureDimentions[1] = 5;
			animation = Animation("resources/graphics/SAster96.bmp", 0.1f, textureDimentions, true, {});
			break;
		default:
			healthPoints = 1;

			textureDimentions[0] = 8;
			textureDimentions[1] = 2;
			animation = Animation("resources/graphics/SAster32.bmp", 0.1f, textureDimentions, true, {});
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
		engine.getLevel().addObject(asteroid);
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
			engine.getLevel().addObject(boom);

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

		healthPoints = 1;

		aux = position.x;

		int textureDimentions[2] = { 8,2 };

		animation = Animation("resources/graphics/drone.bmp", 0.1f, textureDimentions, true, {});
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
			engine.getLevel().addObject(boom);

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
					float phaseOffset = myPeasents.size() * 0.2f;
					myPeasents.push_back(peasent);
					peasent->position.x = position.x + phaseOffset;
					peasent->position.y = position.y;
					peasent->phaseOffset = phaseOffset;
					engine.getLevel().addObject(peasent);
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
			engine.getLevel().addObject(entity);
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
			engine.getLevel().addObject(entity);
			time = 0;
		}
	}
};

class ally : public Pawn {
public:
	ally(bool visibility = true, bool isBullet = false, bool hasSense = false)
		: Pawn(visibility, isBullet, hasSense) {
	}
	int shipHealthMax = 5;
	int shipHealth = 5;

	bool keyPressed = false;

	int firePower = 0;

	int positionOffset = 0;

	struct
	{
		int x = 0;
		int y = 0;
	}bulletOffset;


	void TakeShipDamage() {
		if (damageCooldown <= 0)
		{
			//std::cout << "Ship Damaged" << std::endl;
			shipHealth -= 1;
			damageCooldown = damageCooldownDefault;
		}
	}

	void checkDamageCooldown() {
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
				engine.getLevel().addObject(bullet);
				keyPressed = true;
			}
		}
		else {
			keyPressed = false;
		}
	};
private:
	float damageCooldownDefault = 1;
	float damageCooldown = 0;
};

class spaceship : public ally {
public:
	spaceship(bool visibility = true, bool isBullet = false, bool hasSense = false)
		: ally(visibility, isBullet, hasSense) {
	}

	int textureDimentions[2] = { 7,1 };

	std::string currentAnimation = "";
	int animationState = 0;

	bool isGameOver = false;

	bool canTakeDamage = true;
	float damageCooldown = 0;


	void OnStart() override {

		int textureDimentions[2] = { 7,1 };


		shipHealthMax = 5;
		shipHealth = 5;
		keyPressed = false;
		firePower = 0;

		movementSpeed = 200.0f;

		bulletOffset.x = 0;
		bulletOffset.y = 24;

		animationState = 0;
		objectGroup = "player";

		position.x = 0.0f;
		position.y = -100.0f;

		collisionBoxSize.w = collisionBoxSize.h = 64.0f;
		rotation = *GetGlobalRotation();
	}

	void OnUpdate() override {



		if (isGameOver == false)
		{

			ShootCheck();
			checkDamageCooldown();

			if (input.IsGamepadButtonPressed(GamepadButton::DPadLeft, false)) {
				position.x -= movementSpeed * engine.deltaTime;
				animationState = 2;
			}
			else if (input.IsGamepadButtonPressed(GamepadButton::DPadRight, false)) {
				position.x += movementSpeed * engine.deltaTime;
				animationState = 1;
			}
			else {
				animationState = 0;
			}
			if (input.IsGamepadButtonPressed(GamepadButton::DPadUp, false)) {
				position.y += movementSpeed * engine.deltaTime;
			}
			else if (input.IsGamepadButtonPressed(GamepadButton::DPadDown, false)) {
				position.y -= movementSpeed * engine.deltaTime;
			}
		}

		if (animationState == 1 && currentAnimation != "Right")
		{
			currentAnimation = "Right";

			animation = Animation("resources/graphics/Ship1.bmp", 0.1f, textureDimentions, false, {4,5,6});
			animation.spriteIndex = 0;
		}
		else if (animationState == 2 && currentAnimation != "Left")
		{
			currentAnimation = "Left";
			animation = Animation("resources/graphics/Ship1.bmp", 0.1f, textureDimentions, false, { 2,1,0 });
			animation.spriteIndex = 0;
		}
		else if (animationState == 0 && currentAnimation != "Idle")
		{
			currentAnimation = "Idle";
			animation = Animation("resources/graphics/Ship1.bmp", 0.1f, textureDimentions, false, {3});
			animation.spriteIndex = 0;
		}


		if (shipHealth <= 0 && isGameOver == false) {
			isGameOver = true;

			position.x = 1000.0f;
			position.y = 1000.0f;
		}
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
			std::cout << "Fire Power: " << firePower << std::endl;

		}
	}
	void RecruitCompanion()
	{
		for (int i = 0; i < 2; i++)
		{
			/*if (CompanionListBool[i] == false)
			{
				companionList[i]->recruted = true;
				companionList[i]->SetPosition();

				CompanionListBool[i] = true;

				break;
			}*/
		}

	}

	void OnCollideEnter(GameObject& contact) override {

		int textureDimentions[2] = { 7,3 };

		std::cout << "Contacting: " << contact.objectGroup << std::endl;

		if (contact.objectGroup == "enemyBullet") {
			explosion* boom = new explosion();
			boom->position.x = position.x;
			boom->position.y = position.y;
			if (animationState == 1 && currentAnimation != "Right")
			{
				currentAnimation = "Up";

				animation = Animation("resources/graphics/Ship2.bmp", 0.1f, textureDimentions, false,
					{
					4,5,6, 4,5,6, 4,5,6,
					11,12,13, 11,12,13, 11,12,13,
					18,19,20,18,19,20,18,19,20
					}
				);
				animation.spriteIndex = 0;
			}
			else if (animationState == 2 && currentAnimation != "Left")
			{
				currentAnimation = "Down";
				animation = Animation("resources/graphics/Ship2.bmp", 0.1f, textureDimentions, false,
					{
					0,1,2, 0,1,2,0,1,2,
					9,8,7, 9,8,7,9,8,7,
					16,15,14, 16,15,14,16,15,14
					}
				);
				animation.spriteIndex = 0;
			}
			else if (animationState == 0 && currentAnimation != "Idle")
			{
				currentAnimation = "Idle";
				animation = Animation("resources/graphics/Ship1.bmp", 0.1f, textureDimentions, false,
					{
						3, 10, 17,3, 10, 17,3, 10, 17
					}
				);
				animation.spriteIndex = 0;
			}
			engine.getLevel().addObject(boom);
			TakeShipDamage();
			//std::cout << "Ship Damaged by " << contact.objectGroup << std::endl;
			contact.Destroy();
		}

		if (contact.objectGroup == "enemy") {
			animation = Animation("resources/graphics/Ship2.bmp", 0.1f, textureDimentions, false, { 3, 10, 17,3, 10, 17,3, 10, 17 });
			animation.spriteIndex = 0;
			TakeShipDamage();
			//std::cout << "Ship Damaged by " << contact.objectGroup << std::endl;
		}

		if (contact.objectGroup == "powerUpMissile") {
			UpgradeFirePower();
			contact.Destroy();
		}

		if (contact.objectGroup == "powerUpCompanion") {
			RecruitCompanion();
			std::cout << "Power UP Companion" << std::endl;
			contact.Destroy();
		}

		if (contact.objectGroup == "powerUpHeal") {
			HealShip();
			contact.Destroy();
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
			engine.getLevel().addObject(enemy);
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

			enemy->position.x = getRandomFloat(-290.f, 290.f);
			enemy->position.y = 300.0f;
			engine.getLevel().addObject(enemy);
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
			enemy->position.x = -350.0f;
			enemy->position.y = getRandomFloat(0.f, 205.f);
			engine.getLevel().addObject(enemy);
			time = 0;
		}
	}
};

class SndBackground : public LevelBackground
{
public:

	SndBackground(std::string filepath, float sizeX, float sizeY, float posX, float posY)
		: LevelBackground(filepath, sizeX, sizeY, posX, posY)
	{}

	void OnUpdate() override
	{
		float scrollSpeed = 0.01f;
		scrollRect.h += scrollSpeed * engine.deltaTime;
		//std::cout << scrollRect.h << std::endl;
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

	GameLevel level;

	LevelBackground* backgroundLayer1 = new LevelBackground("resources/graphics/galaxy2.bmp", 2.f, 2.f, 0.f, 0.f);
	SndBackground* backgroundLayer2 = new SndBackground("resources/graphics/PULife.bmp", 0.4f, 0.2f, 1.f, 0.f);
	FirstBackground* firstLayer3 = new FirstBackground("resources/graphics/MAster96.bmp", 2.f, 2.f, -0.5f, 0.f);

	level.setLayerSize(3);
	level.background[0] = backgroundLayer1;
	level.background[1] = firstLayer3;
	level.background[2] = backgroundLayer2;

	engine.setLevel(level);

	spaceship* ship = new spaceship();
	engine.getLevel().addObject(ship);

	rusherSpawner* spawner = new rusherSpawner();
	engine.getLevel().addObject(spawner);

	lonerSpawner* spawner2 = new lonerSpawner();
	engine.getLevel().addObject(spawner2);

	metalAsteroidSpawner* spawner3 = new metalAsteroidSpawner();
	engine.getLevel().addObject(spawner3);
	
	stoneAsteroidSpawner* spawner4 = new stoneAsteroidSpawner();
	engine.getLevel().addObject(spawner4);
	
	droneSpawner* spawner5 = new droneSpawner();
	engine.getLevel().addObject(spawner5);
	
	powerUpCompanion* p1 = new powerUpCompanion();
	p1->position.x = 0.f;
	p1->position.y = 300.f;
	engine.getLevel().addObject(p1);




	engine.Initialize(gameWindow);

}
