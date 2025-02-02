#include "droneSpawner.h"
#include "dronePack.h"
#include "ToolBox.h"
#include "Engine.h"

extern GameEngine::Engine engine;
droneSpawner::droneSpawner(bool visibility, bool isBullet, bool hasSense)
	: GameObject(visibility, isBullet, hasSense) {
	hasBox2d = false;
}

void droneSpawner::OnStart() {
	objectGroup = "RSpwaner";
}

void droneSpawner::OnUpdate(float deltaTime) {
	time += deltaTime;
	if (time > spawnCooldown) {
		dronePack* enemy = new dronePack(true, false, true);
		enemy->position.x = getRandomFloat(-240.f, 240.f);
		enemy->position.y = 300.0f;
		engine.getLevel()->addObject(enemy);
		time = 0;
	}
}