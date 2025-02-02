#pragma once
#include "GameObjects.h"


class metalAsteroidSpawner : public GameObject {
public:
	metalAsteroidSpawner(bool visibility = false, bool isBullet = false, bool hasSense = false);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;

private:
	float spawnCooldown = 10.0f;
	float time = 0.0f;
};

