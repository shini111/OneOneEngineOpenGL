#pragma once
#include "backgroundAssets.h"

class shiftingBackground : public backgroundAssets
{
public:
    shiftingBackground(std::string filepath, float sizeX, float sizeY, float posX, float posY, bool tile, int rows, int columns, int numTilesX, int numTilesY, std::vector<int> tileIDs);

    void resetPosition() override;

private:
    float baseMoveSpeed;
};

