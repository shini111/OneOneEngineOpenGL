#include "enemyProjectile.h"

enemyProjectile::enemyProjectile(bool visibility, bool isBullet, bool hasSense)
	: GameObject(visibility, isBullet, hasSense) {
}

void enemyProjectile::OnStart() {
	int textureDimentions[2] = { 8,1 };
	SetSortingLayer(5);
	animation = new Animation("resources/graphics/EnWeap6.bmp", 0.1f, textureDimentions, true, {});
	objectGroup = "enemyBullet";
	collisionBoxSize.w = collisionBoxSize.h = 16.0f;
}

void enemyProjectile::OnUpdate(float deltaTime) {
	position.y -= moveSpeed * deltaTime;
	if (position.y < -280.f) {
		Destroy();
	}
}