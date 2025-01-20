#pragma once
#include <string>
#include <vector>
#include "GameObjects.h"


class LevelBackground
{
public:
	std::string background_path;
	float scrollingSpeed = 0;
	unsigned int m_ShaderProgram;
	unsigned int m_vao;
	unsigned int m_Texture;
	unsigned int m_ebo;
	unsigned int m_vbo;
	bool isInit = false;
	bool isTiled = false;
	struct
	{
		int rows;
		int columns;
	}tileMapSize;
	float tiledVertices[32];
	std::vector<int> tileIDs; // New member to store tile IDs
	struct
	{
		int x;
		int y;
	} numTiles; // New member to store the number of tiles in x and y directions
	

	LevelBackground()
		: background_path(""), size{ 1.f, 1.f }, scrollRect{ 0.f, 0.f, 0.f, 0.f }, isTiled(false), tileMapSize{ 1, 1 }, numTiles{ 1, 1 }
	{
	}

	LevelBackground(std::string filepath, float sizeX = 1.f, float sizeY = 1.f, float posX = 0.f, float posY = 0.f, bool tile = false, int rows = 1, int columns = 1, int numTilesX = 1, int numTilesY = 1, std::vector<int> tileIDs = {})
		: background_path(filepath), size{ sizeX, sizeY }, scrollRect{ posX, posY, 0.0f, 0.0f }, isTiled(tile), tileMapSize{ rows, columns }, numTiles{ numTilesX, numTilesY }, tileIDs(tileIDs)
	{
	}

	virtual void OnUpdate() {
	};

	struct{
		float x;
		float y;
	}size;

	enum scrollingDirectionEnum {
		vertical,
		horizontal,
	};

	int scrollingDirection = vertical;

	struct
	{
		float w;
		float h;

		float w2 = 0.0f;
		float h2 = 0.0f;
	}scrollRect;

	private:
		void Init();
};

class GameLevel
{
public:
	std::vector<GameObject*> levelObjects;
	std::vector<LevelBackground*> background;

	void setLayerSize(int layerSize);
	void addObject(GameObject* obj);
	

};

