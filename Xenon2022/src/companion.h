#pragma once

#include "ally.h"


class companion : public ally {
public:
	companion(bool visibility = true, bool isBullet = false, bool hasSense = false);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;
	void OnCollideEnter(GameObject& contact) override;

	void HealShip();
	void UpgradeFirePower();

private:
	int textureDimentions[2] = { 4,5 };
};

