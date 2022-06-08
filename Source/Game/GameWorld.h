#pragma once
#include "tga2d/render/RenderCommon.h"
#include "tga2d/model/AnimatedModelInstance.h"
#include "QuadTree/QuadNode.h"
 
#include "../External/Custom/Input.h"

namespace Tga2D
{
	class ModelInstancer;
	class Texture;
}

struct GameObject
{
	Tga2D::Vector2f myPosition;
	Tga2D::Vector2f mySize;

	const bool operator==(const GameObject& aLhs)
	{
		return myPosition == aLhs.myPosition;
	}
};


class GameWorld
{
	

public:
	GameWorld(); 
	~GameWorld();

	void Init();
	void Update(float aTimeDelta); 
	void Render();
private:
	std::shared_ptr<QuadNode<GameObject>> myRoot;
	Tga2D::Vector2f GetLinearCoords(CommonUtilities::Vector2<int> someCoords);

	//Tga2D::AnimatedModelInstance model;
};