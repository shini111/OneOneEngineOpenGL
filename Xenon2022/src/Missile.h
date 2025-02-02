#pragma once

#include "GameObjects.h"
#include "Animation.h"
#include "ToolBox.h"

class missile : public GameObject {
public:
	missile(bool visibility = true, bool isBullet = false, bool hasSense = true);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;
	int getMissileDamage();

	float moveSpeed = 250.0f;
	int firePower = 0;
	int missileDamage = 1;
};