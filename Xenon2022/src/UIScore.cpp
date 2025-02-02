#include "UIScore.h"
#include "InfiniteScrollerLevel.h"
#include "Engine.h"


extern GameEngine::Engine engine; // Declare the external engine object

void UIScore::OnUpdate(float deltaTime)
{
	std::string aux = std::to_string(dynamic_cast<InfiniteScrollerLevel*>(engine.getLevel())->GetScore());
	aux = std::string(10 - aux.length(), '0') + aux;
	std::string aux2 = "Score: \n";
	myText = aux2 + aux;
}