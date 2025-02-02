#pragma once
#include "LevelBackground.h"

class backgroundAssets : public LevelBackground {
public:
	backgroundAssets(std::string filepath, float sizeX, float sizeY, float posX, float posY, bool tile, int rows, int columns, int numTilesX, int numTilesY, std::vector<int> tileIDs)
		: LevelBackground(filepath, sizeX, sizeY, posX, posY, tile, rows, columns, numTilesX, numTilesY, tileIDs) {
	}

	void OnStart() override;
	void OnUpdate(float deltaTime) override;
	virtual void resetPosition();
	float moveSpeed = 50.f;

private:
	float initialYPos;
};