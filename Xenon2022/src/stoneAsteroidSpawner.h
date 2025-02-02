#pragma once

#include "GameObjects.h"


class stoneAsteroidSpawner : public GameObject {
public:
	stoneAsteroidSpawner(bool visibility = false, bool isBullet = false, bool hasSense = false);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;

private:
	float spawnCooldown = 20.0f;
	float time = 0.0f;
	std::vector<int> asteroidSizes = { 32, 64, 96 };
};