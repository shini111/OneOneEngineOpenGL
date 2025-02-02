#include "Enemy.h"



	void Enemy::showDamageFeedback() {
		modulate.r = 255;
		modulate.g = 0;
		modulate.b = 0;
	}

	void Enemy::hideDamageFeedback() {
		modulate.r = 255;
		modulate.g = 255;
		modulate.b = 255;
	}

	void Enemy::checkDamageFeedback() {
		if (damageFeedbackTimer > 0) {
			damageFeedbackTimer -= damageFeedbackDuration * damageFeedbackSpeed * myEngine.GetEngine().deltaTime;

			if (int(damageFeedbackTimer) % 2 == 0) {
				showDamageFeedback();
			}
			else {
				hideDamageFeedback();
			}

		}
		else
		{
			hideDamageFeedback();
			damageFeedbackTimer = 0;
		}
	}

	void Enemy::CreatePowerUp(GameObject* powerUp, float posX, float posY)
	{
		powerUp->position.x = posX;
		powerUp->position.y = posY;
		myEngine.GetEngine().getLevel()->addObject(powerUp);

	}

	void Enemy::TakeDamage(int paramFirePower) {
		healthPoints -= paramFirePower;

		if (healthPoints <= 0)
		{
			if (MyGameEngine::getRandomFloat(0.f, 100.f) >= (100.f - dropChance))
			{
				switch (MyGameEngine::getRandomInt(0, 3))
				{
				case 0:

					CreatePowerUp(new powerUpHeal(true, true, true), position.x, position.y);

					break;
				case 1:

					CreatePowerUp(new powerUpMissile(true, true, true), position.x, position.y);

					break;
				case 2:

					CreatePowerUp(new powerUpCompanion(true, true, true), position.x, position.y);

					break;
				default:
					break;
				}
			}
			Destroy();
			dynamic_cast<MyLevel*>(myEngine.GetEngine().getLevel())->AddScore(scoreValue);
		}
		else {

			showDamageFeedback();
			damageFeedbackTimer = damageFeedbackDuration;

		}
	}

	float damageFeedbackTimer = 0;
	float damageFeedbackDuration = 5;
	float damageFeedbackSpeed = 2;

