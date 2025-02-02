#include "Enemy.h"
#include "powerUpHeal.h"
#include "powerUpMissile.h"
#include "powerUpCompanion.h"
#include "Explosion.h"
#include "Engine.h"

extern GameEngine::Engine engine;

Enemy::Enemy(bool visibility, bool isBullet, bool hasSense)
	: GameObject(visibility, isBullet, hasSense) {
}

void Enemy::CreatePowerUp(GameObject* powerUp, float posX, float posY) {
	powerUp->position.x = posX;
	powerUp->position.y = posY;
	engine.getLevel()->addObject(powerUp);
}

void Enemy::TakeDamage(int paramFirePower) {
	healthPoints -= paramFirePower;

	if (healthPoints <= 0) {
		if (getRandomFloat(0.f, 100.f) >= (100.f - dropChance)) {
			switch (getRandomInt(0, 3)) {
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
		dynamic_cast<InfiniteScrollerLevel*>(engine.getLevel())->AddScore(scoreValue);
	}
}