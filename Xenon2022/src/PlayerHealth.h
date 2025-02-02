#pragma once
#include "LevelBackground.h"
#include <string>
#include <vector>

class PlayerHealth : public LevelBackground
{
public:
	PlayerHealth(std::string filepath, float sizeX, float sizeY, float posX, float posY, bool tile, int rows = 1, int columns = 1, int numTilesX = 3, int numTilesY = 1, std::vector<int> tileIDs = { 0,0,0,0,0 })
		: LevelBackground(filepath, sizeX, sizeY, posX, posY, tile, rows, columns, numTilesX, numTilesY, tileIDs)
	{
	}

	void OnStart() override;
	void UpdateHPBar(float healthPercent);

private:
	float maxTiles;
};

