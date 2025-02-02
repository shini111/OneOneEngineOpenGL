#include "lonerSpawner.h"
#include "loner.h"
#include "ToolBox.h"
#include "Engine.h"
extern GameEngine::Engine engine;

lonerSpawner::lonerSpawner(bool visibility, bool isBullet, bool hasSense)
	: GameObject(visibility, isBullet, hasSense) {
}

void lonerSpawner::OnStart() {
	objectGroup = "LSpwaner";
	position.x = 0;
}

void lonerSpawner::OnUpdate(float deltaTime) {
	time += 1 * deltaTime;
	if (time > spawnCooldown) {
		loner* enemy = new loner(true, false, true);
		enemy->SetSortingLayer(3);
		enemy->position.x = -350.0f;
		enemy->position.y = getRandomFloat(0.f, 205.f);
		engine.getLevel()->addObject(enemy);
		time = 0;
	}
}