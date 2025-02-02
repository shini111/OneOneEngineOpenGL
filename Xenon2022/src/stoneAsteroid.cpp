#include "stoneAsteroid.h"
#include "Animation.h"
#include "explosion.h"
#include "missile.h"
#include "ToolBox.h"
#include "Engine.h"

extern GameEngine::Engine engine;

stoneAsteroid::stoneAsteroid(bool visibility, bool isBullet, bool hasSense)
	: Enemy(visibility, isBullet, hasSense) {
}

void stoneAsteroid::OnStart() {
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
	rotation = *GetGlobalRotation();
}

void stoneAsteroid::OnUpdate(float deltaTime) {
	position.x += moveSpeed.x * deltaTime;
	position.y -= moveSpeed.y * deltaTime;

	if (position.x > 350 || position.x < -350) {
		Destroy();
	}

	if (position.y < -300) {
		Destroy();
	}
}

void stoneAsteroid::OnCollideEnter(GameObject& contact) {
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

void stoneAsteroid::OnDestroyed() {
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

void stoneAsteroid::createAsteroid(float posX, float posY, float size, float speedX, float speedY) {
	stoneAsteroid* asteroid = new stoneAsteroid();
	asteroid->position.x = posX;
	asteroid->position.y = posY;
	asteroid->asteroidSize = size;
	asteroid->moveSpeed.x = speedX;
	asteroid->moveSpeed.y = speedY;
	engine.getLevel()->addObject(asteroid);
}