#include "metalAsteroid.h"
#include "Animation.h"
#include "ToolBox.h"


metalAsteroid::metalAsteroid(bool visibility, bool isBullet, bool hasSense)
	: GameObject(visibility, isBullet, hasSense) {
	asteroidSize = asteroidTypes[getRandomInt(0, 3)];
}

void metalAsteroid::OnStart() {
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
	rotation = *GetGlobalRotation();
}

void metalAsteroid::OnUpdate(float deltaTime) {
	position.y -= moveSpeed * deltaTime;
	if (position.y < -300) {
		Destroy();
	}
}

void metalAsteroid::OnCollideEnter(GameObject& contact) {
	if (contact.objectGroup == "bullet") {
		contact.Destroy();
	}
}