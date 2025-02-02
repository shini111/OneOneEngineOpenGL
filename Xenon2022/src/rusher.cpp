#include "rusher.h"
#include "Engine.h"


extern GameEngine::Engine engine;

rusher::rusher(bool visibility, bool isBullet, bool hasSense)
	: Enemy(visibility, isBullet, hasSense) {
}

void rusher::OnStart() {
	healthPoints = 2;
	scoreValue = 5;
	int textureDimentions[2] = { 4,6 };
	animation = new Animation("resources/graphics/rusher.bmp", 0.05f, textureDimentions, true, {});
	objectGroup = "enemy";
	collisionBoxSize.w = 48.0f;
	collisionBoxSize.h = 32.0f;
	rotation = *GetGlobalRotation();
}

void rusher::OnUpdate(float deltaTime) {
	position.y -= moveSpeed * deltaTime;
	if (position.y < -300) {
		Destroy();
	}
}

void rusher::OnCollideEnter(GameObject& contact) {
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