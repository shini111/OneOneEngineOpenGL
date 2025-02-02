#include "GameLevel.h"


void GameLevel::addObject(GameObject* obj)
{
	levelObjects.push_back(obj);
	obj->OnStart();
}

void GameLevel::addUIText(UIText* uiText)
{
	uiTexts.push_back(uiText);
}

void GameLevel::addBackground(LevelBackground* background)
{
	levelbackgrounds.push_back(background);
	background->OnStart();
}


void GameLevel::SetSortingLayerSize(int i)
{
	listOfLayers.resize(i);
}