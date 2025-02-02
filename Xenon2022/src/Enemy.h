#pragma once

#include "GameObjects.h"
#include "ToolBox.h"
#include "InfiniteScrollerLevel.h"

class Enemy : public GameObject {
public:
	Enemy(bool visibility = true, bool isBullet = false, bool hasSense = false);

	void CreatePowerUp(GameObject* powerUp, float posX, float posY);
	void TakeDamage(int paramFirePower);

	int scoreValue = 10;
	int healthPoints = 1;
	float dropChance = 10.f;
};