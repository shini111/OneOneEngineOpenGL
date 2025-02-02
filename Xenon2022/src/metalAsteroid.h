#pragma once
#include "GameObjects.h"


class metalAsteroid : public GameObject {
public:
	metalAsteroid(bool visibility = true, bool isBullet = true, bool hasSense = true);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;
	void OnCollideEnter(GameObject& contact) override;

private:
	float moveSpeed = 60.0f;
	std::vector<int> asteroidTypes = { 32, 64, 96 };
	int asteroidSize;
};

