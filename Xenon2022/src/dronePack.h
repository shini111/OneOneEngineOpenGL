#pragma once
#include "GameObjects.h"
#include "drone.h"


class dronePack : public GameObject {
public:
	dronePack(bool visibility = false, bool isBullet = false, bool hasSense = false);
	void OnUpdate(float deltaTime) override;

private:
	int myDroneNumber = 10;
	std::vector<drone*> myPeasents;
	float time = 0.f;
	float spawnCooldown = 0.3f;
};
