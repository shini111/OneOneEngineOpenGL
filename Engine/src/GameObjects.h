#pragma once
#include <string>
#include "Animation.h"
#include "Objects.h"

typedef struct b2BodyId;
typedef struct b2BodyDef;
typedef struct b2ShapeId;
typedef struct b2ShapeDef;
typedef struct b2Polygon;

class GameObject : public Object
{
public:
	GameObject(bool visibility, bool isBullet, bool hasSense)
			: visible(visibility), isBullet(isBullet), hasSense(hasSense) {
	}

	~GameObject()
	{
		delete bodyId;
		delete bodyDef;
		delete shapeId;
		delete shapeDef;
		delete boxCollision;
		
		bodyId = nullptr;
		bodyDef = nullptr;
		shapeId = nullptr;
		shapeDef = nullptr;
		boxCollision = nullptr;
	}
	unsigned int m_ShaderProgram;
	unsigned int m_vao;
	unsigned int m_Texture;
	unsigned int m_ebo;
	unsigned int m_vbo;
	bool isInit = false;

	float m_Vertices[32];
	float elapsedTime = 0.f;

	bool hasBox2d = true;
	bool box2dCreated = false;


	Animation* animation;

	void SetColor(float r, float g, float b, float a)
	{
		colorChange.r = r;
		colorChange.g = g;
		colorChange.b = b;
		colorChange.a = a;
	}

	struct
	{
		float r = 0.f;
		float g = 0.f;
		float b = 0.f;
		float a = 0.f;
	}colorChange;

	struct {
		float x = 0.0f;
		float y = 0.0f;
	}position;

	struct {
		float w = 32.0f;
		float h = 32.0f;
	}collisionBoxSize;

	float rotation = 0;

	bool visible = true;
	bool isBullet = false;
	bool hasSense = false;
	bool isContact = false;

	void OnStart() override{};
	void OnUpdate(float deltaTime) override{};
	virtual void OnAnimationFinish() {};
	virtual void OnCollideEnter(GameObject& contact) {};
	void Destroy();
	void OnDestroyed() override{};

	Type getType() const override { return Type::GameObject; }

	std::string objectGroup;

	b2BodyId* bodyId = nullptr;
	b2BodyDef* bodyDef = nullptr;
	b2ShapeId* shapeId = nullptr;
	b2ShapeDef* shapeDef = nullptr;
	b2Polygon* boxCollision = nullptr;


	bool toBeCreated = true;
	bool toBeDeleted = false;
};