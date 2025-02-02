#pragma once
#include <string>
#include <vector>
#include "GameObjects.h"
#include "Objects.h"
#include "LevelBackground.h"
#include "UIText.h"

class GameLevel
{
public:
	std::vector<GameObject*> levelObjects;
	std::vector<LevelBackground*> levelbackgrounds;
	std::vector<UIText*> uiTexts;

	std::vector<std::vector<Object*>> listOfLayers;
	GameLevel() : listOfLayers(8) {};

	virtual ~GameLevel() = default;

	void addObject(GameObject* obj);
	void addUIText(UIText* uitext);
	void addBackground(LevelBackground* background);

protected:
	virtual void SetSortingLayerSize(int i);

};



