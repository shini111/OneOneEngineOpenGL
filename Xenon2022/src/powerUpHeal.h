#pragma once
#include "GameObjects.h"
#include <string>
#include "ToolBox.h"

class powerUpHeal : public GameObject {
public:
	powerUpHeal(bool visibility = true, bool isBullet = true, bool hasSense = true);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;

private:
	float moveSpeed = 30.0f;
};
