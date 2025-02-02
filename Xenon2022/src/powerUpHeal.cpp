#include "powerUpHeal.h"
#include "Engine.h"

extern GameEngine::Engine engine;

powerUpHeal::powerUpHeal(bool visibility, bool isBullet, bool hasSense)
	: GameObject(visibility, isBullet, hasSense) {
}

void powerUpHeal::OnStart() {
	int textureDimentions[2] = { 4,2 };
	SetSortingLayer(6);
	animation = new Animation("resources/graphics/PUShield.bmp", 0.1f, textureDimentions, true, {});
	objectGroup = "powerUpHeal";
}

void powerUpHeal::OnUpdate(float deltaTime) {
	position.y -= moveSpeed * deltaTime;

	if (position.y < -280) {
		Destroy();
	}
}