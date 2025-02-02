#include "loner.h"
#include "enemyProjectile.h"
#include "explosion.h"
#include "missile.h"
#include "ToolBox.h"
#include "Engine.h"

extern GameEngine::Engine engine;

loner::loner(bool visibility, bool isBullet, bool hasSense)
	: Enemy(visibility, isBullet, hasSense) {
}

void loner::OnStart() {
	scoreValue = 7;
	healthPoints = 3;
	int textureDimentions[2] = { 4,4 };
	animation = new Animation("resources/graphics/LonerA.bmp", 0.05f, textureDimentions, true, {});
	objectGroup = "enemy";
	collisionBoxSize.w = collisionBoxSize.h = 64.0f;
	rotation = *GetGlobalRotation();
}

void loner::OnUpdate(float deltaTime) {
	time += 1 * deltaTime;
	if (time > timeCooldown) {
		enemyProjectile* enemyProj = new enemyProjectile();
		enemyProj->position.x = position.x - 10;
		enemyProj->position.y = position.y - 35;
		engine.getLevel()->addObject(enemyProj);
		time = 0;
	}
	position.x += moveSpeed * deltaTime;
	if (position.x > 360) {
		Destroy();
	}
}

void loner::OnCollideEnter(GameObject& contact) {
	if (contact.objectGroup == "bullet") {
		explosion* boom = new explosion(true, false, false);
		boom->position.x = position.x;
		boom->position.y = position.y;
		engine.getLevel()->addObject(boom);

		if (missile* missileContact = dynamic_cast<missile*>(&contact)) {
			int missileFirePower = missileContact->getMissileDamage();
			TakeDamage(missileFirePower);
		}
		contact.Destroy();
	}
}