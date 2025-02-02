#pragma once
#include "Enemy.h"

class drone : public Enemy {
public:
	drone(bool visibility = true, bool isBullet = false, bool hasSense = true);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;
	void OnCollideEnter(GameObject& contact) override;
	float phaseOffset;

private:
	float moveSpeed = 100.0f;
	int packID = 0;
	float elapsedTime = 0.f;
	float sinValue = 0.f;
	float aux = 0.f;
};

