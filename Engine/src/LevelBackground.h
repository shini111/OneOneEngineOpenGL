#pragma once
#include "Objects.h"
#include <string>
#include <vector>

class LevelBackground : public Object
{
public:

	LevelBackground()
		: background_path(""), size{ 1.f, 1.f }, position{ 0.f, 0.f }, isTiled(false), tileMapSize{ 1, 1 }, numTiles{ 1, 1 }
	{
	}

	LevelBackground(std::string filepath, float sizeX = 1.f, float sizeY = 1.f, float posX = 0.f, float posY = 0.f, bool tile = false, int rows = 1, int columns = 1, int numTilesX = 1, int numTilesY = 1, std::vector<int> tileIDs = {})
		: background_path(filepath), size{ sizeX, sizeY }, position{ posX, posY }, isTiled(tile), tileMapSize{ rows, columns }, numTiles{ numTilesX, numTilesY }, tileIDs(tileIDs)
	{
	}

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

	float tiledVertices[48];
	std::vector<int> tileIDs; // New member to store tile IDs
	struct
	{
		int x;
		int y;
	} numTiles; // New member to store the number of tiles in x and y directions


	void OnStart() override {};
	void OnUpdate(float deltaTime) override {};

	void SetColor(float r, float g, float b, float a)
	{
		colorChange.r = r;
		colorChange.g = g;
		colorChange.b = b;
		colorChange.a = a;
	}

	Type getType() const override { return Type::LevelBackground; }

	struct {
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
		float x;
		float y;
	}position;

	struct
	{
		float r = 0.f;
		float g = 0.f;
		float b = 0.f;
		float a = 0.f;

	}colorChange;
};


