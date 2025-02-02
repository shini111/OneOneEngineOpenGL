#pragma once
#include "Engine.h"
#include "MyEngine.h"
#include "PowerUps.h"
#include "MyLevel.h"

class Enemy : public GameObject {
public:
	Enemy(bool visibility = true, bool isBullet = false, bool hasSense = false)
		: GameObject(visibility, isBullet, hasSense) {
	}
	int scoreValue = 10;
	MyGameEngine* engine = MyGameEngine::GetInstance();
	int healthPoints = 1;
	float dropChance = 10.f;
	void showDamageFeedback();
	void hideDamageFeedback();

	void checkDamageFeedback();

	void CreatePowerUp(GameObject* powerUp, float posX, float posY);
	void TakeDamage(int paramFirePower);
private:
	float damageFeedbackTimer = 0;
	float damageFeedbackDuration = 5;
	float damageFeedbackSpeed = 2;

};