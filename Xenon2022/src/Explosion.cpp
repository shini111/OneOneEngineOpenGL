#include "Explosion.h"

explosion::explosion(bool visibility, bool isBullet, bool hasSense)
	: GameObject(visibility, isBullet, hasSense) {
}

void explosion::OnStart() {
	int textureDimentions[2] = { 5,2 };
	SetSortingLayer(5);
	animation = new Animation("resources/graphics/explode64.bmp", 0.1f, textureDimentions, false, {});
}

void explosion::OnAnimationFinish() {
	Destroy();
}