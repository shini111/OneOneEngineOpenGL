#pragma once
#include "GameObjects.h"
#include <string>
#include "ToolBox.h"

class powerUpCompanion : public GameObject {
public:
	powerUpCompanion(bool visibility = true, bool isBullet = true, bool hasSense = true);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;

private:
	float moveSpeed = 30.0f;
};