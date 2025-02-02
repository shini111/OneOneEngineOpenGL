#include "PowerUps.h"

//Power UP Missile
void powerUpMissile::OnStart() {
	int textureDimentions[2] = { 4,2 };
	SetSortingLayer(6);
	animation = new Animation("resources/graphics/PUWeapon.bmp", 0.1f, textureDimentions, true, {});
	objectGroup = "powerUpMissile";
	collisionBoxSize.w = 32.0f;
	collisionBoxSize.h = 32.0f;

}

void powerUpMissile::OnUpdate() {
	position.y -= moveSpeed * engine->GetEngine().deltaTime;

	if (position.y < -280) {
		Destroy();
	}
}
//Power Up Heal
void powerUpHeal::OnStart() {
	int textureDimentions[2] = { 4,2 };
	SetSortingLayer(6);
	animation = new Animation("resources/graphics/PUShield.bmp", 0.1f, textureDimentions, true, {});
	objectGroup = "powerUpHeal";

}

void powerUpHeal::OnUpdate() {
	position.y -= moveSpeed * engine->GetEngine().deltaTime;

	if (position.y < -280) {
		Destroy();
	}
}

//Power Up Companion
void powerUpCompanion::OnStart() {
	int textureDimentions[2] = { 4,5 };
	SetSortingLayer(6);
	animation = new Animation("resources/graphics/clone.bmp", 0.1f, textureDimentions, true, { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 });
	objectGroup = "powerUpCompanion";

}

void powerUpCompanion::OnUpdate() {

	position.y -= moveSpeed * engine->GetEngine().deltaTime;

	if (position.y < -280) {
		Destroy();
	}
}