#pragma once
#include "Engine.h"
#include "MyEngine.h"


class powerUpMissile : public GameObject {
public:
	powerUpMissile(bool visibility = true, bool isBullet = true, bool hasSense = true)
		: GameObject(visibility, isBullet, hasSense) {
	}

	MyGameEngine* engine = MyGameEngine::GetInstance();
	float moveSpeed = 30.0f;


	void OnStart() override;

	void OnUpdate() override;

};

class powerUpHeal : public GameObject {
public:
	powerUpHeal(bool visibility = true, bool isBullet = true, bool hasSense = true)
		: GameObject(visibility, isBullet, hasSense) {
	}
	MyGameEngine* engine = MyGameEngine::GetInstance(); MyGameEngine myEngine;
	float moveSpeed = 30.0f;


	void OnStart() override;

	void OnUpdate() override;

};

class powerUpCompanion : public GameObject {
public:
	powerUpCompanion(bool visibility = true, bool isBullet = true, bool hasSense = true)
		: GameObject(visibility, isBullet, hasSense) {
	}
	MyGameEngine* engine = MyGameEngine::GetInstance();
	float moveSpeed = 30.0f;


	void OnStart() override;

	void OnUpdate() override;

};
