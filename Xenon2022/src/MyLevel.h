#pragma once
#include "Engine.h"

class MyLevel : public GameLevel
{
public:
	MyLevel(int i)
	{
		SetSortingLayerSize(i);
	}

	void AddScore(int i);

	int GetScore();

private:
	int myScore = 10;


};
