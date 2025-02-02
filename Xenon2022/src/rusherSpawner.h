#pragma once

#include "GameObjects.h"


class rusherSpawner : public GameObject {
public:
	rusherSpawner(bool visibility = false, bool isBullet = false, bool hasSense = false);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;

private:
	float spawnCooldown = 2.0f;
	float time = 0.0f;
};