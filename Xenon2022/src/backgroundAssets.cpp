#include "backgroundAssets.h"


void backgroundAssets::OnStart() {
	initialYPos = position.y;
}

void backgroundAssets::OnUpdate(float deltaTime) {
	position.y -= moveSpeed * deltaTime;

	if (position.y < -500.f) {
		resetPosition();
	}
}

void backgroundAssets::resetPosition() {
	position.y = initialYPos;
}