#pragma once
#include "Enemy.h"


class stoneAsteroid : public Enemy {
public:
	stoneAsteroid(bool visibility = true, bool isBullet = false, bool hasSense = true);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;
	void OnCollideEnter(GameObject& contact) override;
	void OnDestroyed() override;
	int asteroidSize;

private:
	void createAsteroid(float posX, float posY, float size, float speedX, float speedY);

	struct {
		float x = 0.0f;
		float y = 32.0f;
	} moveSpeed;


};
