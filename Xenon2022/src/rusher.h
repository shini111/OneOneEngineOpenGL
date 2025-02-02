#pragma once

#include "Enemy.h"
#include "GameObjects.h"
#include "explosion.h"
#include "missile.h"

class rusher : public Enemy {
public:
	rusher(bool visibility = true, bool isBullet = false, bool hasSense = true);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;
	void OnCollideEnter(GameObject& contact) override;

private:
	float moveSpeed = 150.0f;
};