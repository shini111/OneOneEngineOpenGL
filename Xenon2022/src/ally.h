#pragma once
#include "GameObjects.h"
#include "missile.h"
#include "ToolBox.h"

class ally : public GameObject {
public:
	ally(bool visibility = true, bool isBullet = false, bool hasSense = false);

	virtual void TakeShipDamage();
	virtual void checkDamageCooldown(float deltaTime);
	void ShootCheck();
	float shipHealth = 5.f;

protected:
	float shipHealthMax = 5.f;
	bool keyPressed = false;
	int firePower = 0;
	int positionOffset = 0;
	float movementSpeed;
	struct {
		int x = 0;
		int y = 0;
	} bulletOffset;
	float damageCooldownDefault = 1;
	float damageCooldown = 0;
};

