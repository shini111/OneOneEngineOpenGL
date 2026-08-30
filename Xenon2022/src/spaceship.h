#pragma once
#include "ally.h"
#include "PlayerLife.h"
#include "PlayerHealth.h"
#include "companion.h"
#include <vector>
#include <string>

class spaceship : public ally {
public:
	spaceship(bool visibility = true, bool isBullet = false, bool hasSense = false);

	void OnStart() override;
	void OnUpdate(float deltaTime) override;
	void OnCollideEnter(GameObject& contact) override;
	void OnAnimationFinish() override;

	void TakeShipDamage() override;
	void HealShip();
	void UpgradeFirePower();
	void RecruitCompanion();
	void UpdateHealthPercent();

private:
	int textureDimentions[2] = { 7,3 };
	std::string currentAnimation = "";
	int animationState = 0;
	bool isGameOver = false;
	std::vector<float> companionOffset = { 65.f, -65.f };
	bool canTakeDamage = true;
	float damageCooldown = 0;
	bool onAnimation = false;
	float limits[4] = { 290.f, -210.f, -290.f, 210.f };
	int lives = 3;
	std::vector<companion*> myCompanions;
	PlayerLife* playerLifesUI;
	PlayerHealth* playerHealthUI;
	struct {
		float x = 0;
		float y = 0;
	} respawnPosition;
};