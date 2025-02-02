#include "companion.h"
#include "Animation.h"
#include "explosion.h"
#include "ToolBox.h"
#include "Engine.h"

extern GameEngine::Engine engine;

companion::companion(bool visibility, bool isBullet, bool hasSense)
	: ally(visibility, isBullet, hasSense) {
}

void companion::OnStart() {
	shipHealthMax = 3;
	shipHealth = 3;
	keyPressed = false;
	firePower = 0;
	SetSortingLayer(4);
	bulletOffset.x = 0;
	bulletOffset.y = 9;
	animation = new Animation("resources/graphics/clone.bmp", 0.1f, textureDimentions, true, { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 });
	objectGroup = "companion";
	collisionBoxSize.w = collisionBoxSize.h = 32.0f;
	rotation = *GetGlobalRotation();
}

void companion::OnUpdate(float deltaTime) {
	ShootCheck();
	checkDamageCooldown(deltaTime);

	if (shipHealth <= 0) {
		Destroy();
		isInit = false;
		animation = new Animation("resources/graphics/clone.bmp", 0.1f, textureDimentions, false, { 16,17,18,19 });
	}
}

void companion::OnCollideEnter(GameObject& contact) {
	if (contact.objectGroup == "enemyBullet") {
		explosion* boom = new explosion();
		boom->position.x = position.x;
		boom->position.y = position.y;
		isInit = false;
		animation = new Animation("resources/graphics/clone.bmp", 1.f, textureDimentions, false, { 19 });
		engine.getLevel()->addObject(boom);
		TakeShipDamage();
		contact.Destroy();
	}

	if (contact.objectGroup == "enemy") {
		isInit = false;
		animation = new Animation("resources/graphics/clone.bmp", 0.1f, textureDimentions, false, { 19 });
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

void companion::HealShip() {
	int healAmount = 2;
	shipHealth += healAmount;
	if (shipHealth > shipHealthMax) {
		shipHealth = shipHealthMax;
	}
}

void companion::UpgradeFirePower() {
	if (firePower < 2) {
		firePower++;
	}
}