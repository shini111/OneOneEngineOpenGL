#include "metalAsteroidSpawner.h"
#include "metalAsteroid.h"
#include "ToolBox.h"
#include "Engine.h"

extern GameEngine::Engine engine;

metalAsteroidSpawner::metalAsteroidSpawner(bool visibility, bool isBullet, bool hasSense)
	: GameObject(visibility, isBullet, hasSense) {
}

void metalAsteroidSpawner::OnStart() {
	objectGroup = "MASpwaner";
}

void metalAsteroidSpawner::OnUpdate(float deltaTime) {
	time += deltaTime;
	if (time > spawnCooldown) {
		metalAsteroid* entity = new metalAsteroid(true, false, true);
		entity->position.x = getRandomFloat(-290.f, 290.f);
		entity->position.y = 300.0f;
		engine.getLevel()->addObject(entity);
		time = 0;
	}
}