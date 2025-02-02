#include "shiftingBackground.h"
#include "ToolBox.h"

// Constructor definition
shiftingBackground::shiftingBackground(std::string filepath, float sizeX, float sizeY, float posX, float posY, bool tile, int rows, int columns, int numTilesX, int numTilesY, std::vector<int> tileIDs)
	: backgroundAssets(filepath, sizeX, sizeY, posX, posY, tile, rows, columns, numTilesX, numTilesY, tileIDs), baseMoveSpeed(moveSpeed) {
}

// Override resetPosition method
void shiftingBackground::resetPosition() {
	backgroundAssets::resetPosition();
	position.x = getRandomFloat(-280.f, 280.f);
	moveSpeed = baseMoveSpeed;
	moveSpeed += getRandomFloat(-10.f, 10.f);
}