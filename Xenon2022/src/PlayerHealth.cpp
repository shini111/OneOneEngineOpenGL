#include "PlayerHealth.h"

void PlayerHealth::OnStart()
{
	maxTiles = numTiles.x;
}

void PlayerHealth::UpdateHPBar(float healthPercent)
{
	if (healthPercent > 50.f)
	{
		SetColor(0.f, 1.f, 0.f, 1.f);
	}
	else if (healthPercent > 25.f)
	{
		SetColor(1.f, 1.f, 0.f, 1.f);
	}
	else
	{
		SetColor(1.f, 0.f, 0.f, 1.f);
	}
	numTiles.x = (maxTiles * healthPercent) / 100.f;
}