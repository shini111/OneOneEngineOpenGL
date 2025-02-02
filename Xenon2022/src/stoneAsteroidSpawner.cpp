#include "stoneAsteroidSpawner.h"
#include "stoneAsteroid.h"
#include "ToolBox.h"
#include "Engine.h"


extern GameEngine::Engine engine;

stoneAsteroidSpawner::stoneAsteroidSpawner(bool visibility, bool isBullet, bool hasSense)
	: GameObject(visibility, isBullet, hasSense) {
}

void stoneAsteroidSpawner::OnStart() {
	objectGroup = "SASpwaner";
}

void stoneAsteroidSpawner::OnUpdate(float deltaTime) {
	time += deltaTime;
	if (time > spawnCooldown) {
		stoneAsteroid* entity = new stoneAsteroid(true, false, true);
		entity->asteroidSize = asteroidSizes[getRandomInt(0, 3)];
		entity->position.x = getRandomFloat(-280, 280);
		entity->position.y = 300.f;
		engine.getLevel()->addObject(entity);
		time = 0;
	}
}