#pragma once
#include "Enemy.h"


class loner : public Enemy {
public:
	loner(bool visibility = true, bool isBullet = false, bool hasSense = true);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;
	void OnCollideEnter(GameObject& contact) override;

private:
	float moveSpeed = 70.0f;
	float time = 0.0f;
	float timeCooldown = 2.0f;
};
