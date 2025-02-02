#include "drone.h"
#include "Animation.h"
#include "explosion.h"
#include "missile.h"
#include "Engine.h"


extern GameEngine::Engine engine;

drone::drone(bool visibility, bool isBullet, bool hasSense)
	: Enemy(visibility, isBullet, hasSense) {
}

void drone::OnStart() {
	scoreValue = 2;
	healthPoints = 1;
	aux = position.x;
	int textureDimentions[2] = { 8,2 };
	animation = new Animation("resources/graphics/drone.bmp", 0.1f, textureDimentions, true, {});
	objectGroup = "enemy";
}

void drone::OnUpdate(float deltaTime) {
	position.y -= moveSpeed * deltaTime;
	elapsedTime += deltaTime;
	sinValue = (sin(4.f * elapsedTime) * 0.6f);
	position.x = aux + (sin(4.f * elapsedTime) * 40.f);
	if (position.y < -280) {
		Destroy();
	}
}

void drone::OnCollideEnter(GameObject& contact) {
	if (contact.objectGroup == "bullet") {
		explosion* boom = new explosion();
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