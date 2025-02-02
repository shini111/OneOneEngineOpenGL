#include "rusherSpawner.h"
#include "rusher.h"
#include "ToolBox.h"
#include "Engine.h"

extern GameEngine::Engine engine;

rusherSpawner::rusherSpawner(bool visibility, bool isBullet, bool hasSense)
	: GameObject(visibility, isBullet, hasSense) {
	hasBox2d = false;
}

void rusherSpawner::OnStart() {
	objectGroup = "RSpwaner";
}

void rusherSpawner::OnUpdate(float deltaTime) {
	time += 1 * deltaTime;
	if (time > spawnCooldown) {
		rusher* enemy = new rusher(true, false, true);
		enemy->SetSortingLayer(3);
		enemy->position.x = getRandomFloat(-290.f, 290.f);
		enemy->position.y = 300.0f;
		engine.getLevel()->addObject(enemy);
		time = 0;
	}
}
