#pragma once
#include "Engine.h"
#include "MyEngine.h"

class missile : public GameObject {
public:

	missile(bool visibility = true, bool isBullet = true, bool hasSense = true)
		: GameObject(visibility, isBullet, hasSense)
	{

	}

	MyGameEngine myEngine;
	float moveSpeed = 250.0f;
	int firePower = 0;
	int missileDamage = 1;

	int getMissileDamage();

	void OnStart() override;
	void OnUpdate() override;
};
