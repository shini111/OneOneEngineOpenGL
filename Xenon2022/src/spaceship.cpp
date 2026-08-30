#include "spaceship.h"
#include "Animation.h"
#include "explosion.h"
#include "Engine.h"

extern GameEngine::Engine engine;

spaceship::spaceship(bool visibility, bool isBullet, bool hasSense)
	: ally(visibility, isBullet, hasSense) {
}

void spaceship::OnStart() {
	shipHealthMax = 10;
	shipHealth = 10;
	keyPressed = false;
	firePower = 0;
	movementSpeed = 200.0f;
	bulletOffset.x = 0;
	bulletOffset.y = 24;
	animationState = 0;
	objectGroup = "player";
	respawnPosition.x = position.x = 0.0f;
	respawnPosition.y = position.y = -100.0f;
	collisionBoxSize.w = collisionBoxSize.h = 64.0f;
	rotation = *GetGlobalRotation();

	std::vector<int> lifeTiles = { 0, 0, 0, 0, 0 };
	playerLifesUI = new PlayerLife("resources/graphics/PULife.bmp", 0.15f, 0.17f, -290.f, -160.f, true, 1, 1, lives, 1, lifeTiles);
	playerLifesUI->SetSortingLayer(9);
	playerLifesUI->UpdateNumberOfLives(lives);
	engine.getLevel()->addBackground(playerLifesUI);

	std::vector<int> healthTiles = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	playerHealthUI = new PlayerHealth("resources/graphics/Blocks.bmp", 0.02f, 0.1f, -305.f, -210.f, true, 64, 16, 20, 1, healthTiles);
	playerHealthUI->SetSortingLayer(9);
	playerHealthUI->SetColor(0.f, 1.f, 0.f, 1.f);
	engine.getLevel()->addBackground(playerHealthUI);
}

void spaceship::OnUpdate(float deltaTime) {
	if (isGameOver == false) {
		if (myCompanions.size() > 0) {
			for (int i = 0; i < myCompanions.size(); i++) {
				if (myCompanions[i]->shipHealth <= 0) {
					myCompanions[i]->Destroy();
					myCompanions.erase(myCompanions.begin() + i);
				}
			}
		}

		ShootCheck();
		checkDamageCooldown(deltaTime);

		float moveX = 0.0f;
		float moveY = 0.0f;

		if (input.IsGamepadButtonPressed(GamepadButton::DPadLeft, false)) {
			moveX = -1.0f;
			animationState = 2;
		}
		else if (input.IsGamepadButtonPressed(GamepadButton::DPadRight, false)) {
			moveX = 1.0f;
			animationState = 1;
		}
		else {
			animationState = 0;
		}
		if (input.IsGamepadButtonPressed(GamepadButton::DPadUp, false)) {
			moveY = 1.0f;
		}
		else if (input.IsGamepadButtonPressed(GamepadButton::DPadDown, false)) {
			moveY = -1.0f;
		}

		// Normalize the movement vector if moving diagonally
		float magnitude = std::sqrt(moveX * moveX + moveY * moveY);
		if (magnitude > 0.0f) {
			moveX /= magnitude; // Normalize X
			moveY /= magnitude; // Normalize Y
		}

		// Apply the movement
		float auxX = position.x;
		float auxY = position.y;

		auxX += moveX * movementSpeed * deltaTime;
		auxY += moveY * movementSpeed * deltaTime;

		if (auxX < limits[0] && auxX > limits[2]) {
			position.x += moveX * movementSpeed * deltaTime;
		}

		if (auxY >= limits[1] && auxY <= limits[3]) {
			position.y += moveY * movementSpeed * deltaTime;
		}
	}

	if (myCompanions.size() > 0) {
		for (int i = 0; i < myCompanions.size(); i++) {
			myCompanions[i]->position.x = position.x + companionOffset[i];
			myCompanions[i]->position.y = position.y;
		}
	}

	if (animationState == 1 && currentAnimation != "Right" && onAnimation == false) {
		currentAnimation = "Right";
		animation = new Animation("resources/graphics/Ship2.bmp", 0.1f, textureDimentions, false, { 4,5,6 });
	}
	else if (animationState == 2 && currentAnimation != "Left" && onAnimation == false) {
		currentAnimation = "Left";
		animation = new Animation("resources/graphics/Ship2.bmp", 0.1f, textureDimentions, false, { 2,1,0 });
	}
	else if (animationState == 0 && currentAnimation != "Idle" && onAnimation == false) {
		currentAnimation = "Idle";
		animation = new Animation("resources/graphics/Ship2.bmp", 0.1f, textureDimentions, false, { 3 });
	}

	if (shipHealth <= 0) {
		if (lives > 0) {
			shipHealth = shipHealthMax;
			lives--;
			playerLifesUI->UpdateNumberOfLives(lives);
			UpdateHealthPercent();
			position.x = respawnPosition.x;
			position.y = respawnPosition.y;
		}
		else if (isGameOver == false) {
			if (myCompanions.size() > 0) {
				for (int i = myCompanions.size() - 1; i >= 0; i--) {
					myCompanions[i]->Destroy();
					myCompanions.erase(myCompanions.begin() + i);
				}
			}
			isGameOver = true;
			Destroy();
			playerLifesUI = nullptr;
		}
	}
}

void spaceship::OnCollideEnter(GameObject& contact) {
	if (contact.objectGroup == "enemyBullet") {
		explosion* boom = new explosion();
		boom->position.x = position.x;
		boom->position.y = position.y;
		engine.getLevel()->addObject(boom);
		TakeShipDamage();
		contact.Destroy();
	}

	if (contact.objectGroup == "enemy") {
		TakeShipDamage();
	}

	if (contact.objectGroup == "powerUpMissile") {
		UpgradeFirePower();
		contact.Destroy();
	}

	if (contact.objectGroup == "powerUpCompanion") {
		RecruitCompanion();
		contact.Destroy();
	}

	if (contact.objectGroup == "powerUpHeal") {
		HealShip();
		contact.Destroy();
	}
}

void spaceship::OnAnimationFinish() {
	onAnimation = false;
}

void spaceship::HealShip() {
	int healAmount = 2;
	shipHealth += healAmount;
	UpdateHealthPercent();
	if (shipHealth > shipHealthMax) {
		shipHealth = shipHealthMax;
	}
}

void spaceship::UpgradeFirePower() {
	if (firePower < 2) {
		firePower++;
	}
}

void spaceship::RecruitCompanion() {
	if (myCompanions.size() < 2) {
		companion* companion1 = new companion(true, false, true);
		myCompanions.push_back(companion1);
		engine.getLevel()->addObject(companion1);
	}
}

void spaceship::UpdateHealthPercent() {
	float percentHealth = (shipHealth / shipHealthMax) * 100;
	playerHealthUI->UpdateHPBar(percentHealth);
}

void spaceship::TakeShipDamage() {
	// ally::TakeShipDamage() only decrements shipHealth (with its cooldown check) --
	// it has no idea the UI bar exists, since that's a spaceship-only member. Every
	// call site (OnCollideEnter's "enemyBullet" and "enemy" branches) was relying on
	// remembering to refresh the bar separately, and none of them did, so shipHealth
	// was dropping correctly (death/respawn still worked) while the on-screen bar
	// just sat there unchanged until a heal or respawn happened to reset it. Overriding
	// here means the bar updates automatically no matter what calls TakeShipDamage().
	ally::TakeShipDamage();
	UpdateHealthPercent();
}