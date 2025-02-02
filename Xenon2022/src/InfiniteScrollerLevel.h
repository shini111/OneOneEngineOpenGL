#pragma once
#include "GameLevel.h"

class InfiniteScrollerLevel : public GameLevel
{
public:
	InfiniteScrollerLevel(int sortingLayerSize){
		SetSortingLayerSize(sortingLayerSize);
	};

	void AddScore(int i);

	int GetScore() {
		return myScore;
	};

private:

	int myScore = 10;

};

