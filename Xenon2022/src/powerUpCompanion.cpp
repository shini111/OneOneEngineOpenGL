#include "powerUpCompanion.h"
#include "Engine.h"

extern GameEngine::Engine engine;

powerUpCompanion::powerUpCompanion(bool visibility, bool isBullet, bool hasSense)
	: GameObject(visibility, isBullet, hasSense) {
}

void powerUpCompanion::OnStart() {
	int textureDimentions[2] = { 4,5 };
	SetSortingLayer(6);
	animation = new Animation("resources/graphics/clone.bmp", 0.1f, textureDimentions, true, { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 });
	objectGroup = "powerUpCompanion";
}

void powerUpCompanion::OnUpdate(float deltaTime) {
	position.y -= moveSpeed * deltaTime;

	if (position.y < -280) {
		Destroy();
	}
}