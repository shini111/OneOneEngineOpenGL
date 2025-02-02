#pragma once
#include "Objects.h"
#include <string>
#include <vector>
#include "Font.h"

class UIText : public Object
{
public:

	UIText(Font* font, std::string text, float posX = 0.f, float posY = 0.f, float sizeX = 1.f, float sizeY = 1.f)
		: myFont(font), myText(text), position{ posX, posY }, size{ sizeX, sizeY }
	{
	}

	void OnStart() override {
	};
	void OnUpdate(float deltaTime) override {
	};

	Type getType() const override { return Type::UIText; }

	unsigned int m_ShaderProgram;
	unsigned int m_vao;
	unsigned int m_Texture;
	unsigned int m_ebo;
	unsigned int m_vbo;
	float myVertices[32];
	bool isInit = false;
	std::string myText;
	std::vector<char> charText;

	Font* myFont;

	struct
	{
		float x;
		float y;
	}position;

	struct
	{
		float x;
		float y;
	}size;

};
