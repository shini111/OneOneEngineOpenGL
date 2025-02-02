#pragma once
#include "UIText.h"

class UIScore : public UIText
{
public:
	UIScore(Font* font, std::string text, float posX = 0.f, float posY = 0.f, float sizeX = 1.f, float sizeY = 1.f)
		: UIText(font, text, posX, posY, sizeX, sizeY)
	{
	}

	void OnUpdate(float deltaTime) override;
};

