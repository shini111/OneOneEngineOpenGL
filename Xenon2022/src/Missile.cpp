#include "Missile.h"

void missile::OnStart() {
	int textureDimentions[2] = { 2,3 };
	SetSortingLayer(5);
	switch (firePower) {
	case 0:
		animation = new Animation("resources/graphics/missile.bmp", 0.1f, textureDimentions, true, { 0 ,1 });
		break;
	case 1:
		animation = new Animation("resources/graphics/missile.bmp", 0.1f, textureDimentions, true, { 2,3 });
		break;
	case 2:
		animation = new Animation("resources/graphics/missile.bmp", 0.1f, textureDimentions, true, { 4,5 });
		break;
	default:
		animation = new Animation("resources/graphics/missile.bmp", 0.1f, textureDimentions, true, { 0, 1 });
		break;
	}

	collisionBoxSize.w = collisionBoxSize.h = 16.0f;

	objectGroup = "bullet";
}

int missile::getMissileDamage() {
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

void missile::OnUpdate() {
	position.y += moveSpeed * myEngine.GetEngine().deltaTime;

	if (position.y > 250) {
		Destroy();
	}
}