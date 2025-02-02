#pragma once

#include "GameObjects.h"
#include "Animation.h"

class explosion : public GameObject {
public:
	explosion(bool visibility = true, bool isBullet = false, bool hasSense = false);

	void OnStart() override;
	void OnAnimationFinish() override;
};