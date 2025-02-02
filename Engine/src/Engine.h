#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <array>
#include <Windows.h>
#include <cstdint>
#include "Input.h"
#include "GameObjects.h"
#include "GameLevel.h"

typedef struct b2ShapeId;
typedef struct b2Manifold;
typedef struct b2Vec2;
typedef struct b2WorldDef;
typedef struct b2WorldId;

typedef struct SDL_Window;

extern Input input;



class GameWindow
{
public:
	const char* windowName = "Xenon 2022";
	int windowWidth = 640;
	int windowHeight = 480;
};

namespace GameEngine {
	class Engine
	{
	public:
		float deltaTime = 0.0f;

		void setLevel(GameLevel* level);
		GameLevel* getLevel() { return mainLevel; };
		void print(std::string printText);

		void Update();
		void Initialize(GameWindow windowSettings);
	private:
		void sensorListener();
		void contactListener();
		std::vector<char> isolateChars(const std::string& str);
		int returnCharEnum(char letter);
		GameLevel* mainLevel;
		GameWindow windowDisplay;
		int prevTime = currentTime;
		int currentTime = 0;

		float timeStep = 1.0f / 120.0f;
		int subStepCount = 20;

	};

}
