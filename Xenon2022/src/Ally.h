#pragma once
#include "Engine.h"
#include "MyEngine.h"
#include "Missile.h"


class ally : public Pawn {
public:
	ally(bool visibility = true, bool isBullet = false, bool hasSense = false)
		: Pawn(visibility, isBullet, hasSense) {
	}

	MyGameEngine* engine = MyGameEngine::GetInstance();
	float shipHealthMax = 5.f;
	float shipHealth = 5.f;

	bool keyPressed = false;

	int firePower = 0;

	int positionOffset = 0;

	struct
	{
		int x = 0;
		int y = 0;
	}bulletOffset;


	virtual void TakeShipDamage();

	virtual void checkDamageCooldown();
	void ShootCheck();
protected:
	float damageCooldownDefault = 1;
	float damageCooldown = 0;
};
