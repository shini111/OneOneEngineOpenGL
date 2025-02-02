#include "powerUpMissile.h"
#include "Engine.h"

extern GameEngine::Engine engine;

powerUpMissile::powerUpMissile(bool visibility, bool isBullet, bool hasSense)
	: GameObject(visibility, isBullet, hasSense) {
}

void powerUpMissile::OnStart() {
	int textureDimentions[2] = { 4,2 };
	SetSortingLayer(6);
	animation = new Animation("resources/graphics/PUWeapon.bmp", 0.1f, textureDimentions, true, {});
	objectGroup = "powerUpMissile";
	collisionBoxSize.w = 32.0f;
	collisionBoxSize.h = 32.0f;

	rotation = *GetGlobalRotation();
}

void powerUpMissile::OnUpdate(float deltaTime) {
	position.y -= moveSpeed * deltaTime;

	if (position.y < -280) {
		Destroy();
	}
}