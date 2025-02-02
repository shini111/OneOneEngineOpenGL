#pragma once
#include "GameObjects.h"


class lonerSpawner : public GameObject {
public:
	lonerSpawner(bool visibility = false, bool isBullet = false, bool hasSense = false);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;

private:
	float spawnCooldown = 4.0f;
	float time = 0.0f;
};