#pragma once
#pragma once

#include "GameObjects.h"


class enemyProjectile : public GameObject {
public:
	enemyProjectile(bool visibility = true, bool isBullet = true, bool hasSense = true);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;

private:
	float moveSpeed = 250.0f;
};
