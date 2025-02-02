#include "dronePack.h"
#include "ToolBox.h"
#include "Engine.h"

extern GameEngine::Engine engine;

dronePack::dronePack(bool visibility, bool isBullet, bool hasSense)
	: GameObject(visibility, isBullet, hasSense) {
}

void dronePack::OnUpdate(float deltaTime) {
	if (myDroneNumber != 0) {
		time += 1 * deltaTime;
		if (time > spawnCooldown) {
			drone* peasent = new drone(true, false, true);
			peasent->SetSortingLayer(3);
			float phaseOffset = myPeasents.size() * 0.2f;
			myPeasents.push_back(peasent);
			peasent->position.x = position.x + phaseOffset;
			peasent->position.y = position.y;
			peasent->phaseOffset = phaseOffset;
			engine.getLevel()->addObject(peasent);
			time = 0;
			myDroneNumber--;
		}
	}

	removeNullPointers(myPeasents);

	if (myPeasents.empty() && myDroneNumber == 0) {
		Destroy();
	}
}