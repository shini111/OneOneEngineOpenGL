#pragma once


class Object
{
public:

	virtual void OnStart() {};
	virtual void OnUpdate(float deltaTime) {};
	virtual void OnDestroyed(){};

	Object(int sLayer = 0)
		:sortingLayer(sLayer)
	{
	}

	virtual ~Object() = default;

	enum class Type {
		Object,
		LevelBackground,
		GameObject,
		UIText
	};

	virtual Type getType() const { return Type::Object; }

	void SetSortingLayer(int layer)
	{
		sortingLayer = layer;
	}

	int GetSortingLayer()
	{
		return sortingLayer;
	}

protected:
	int sortingLayer = 0;
};
