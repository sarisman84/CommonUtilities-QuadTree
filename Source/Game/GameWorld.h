#pragma once
#include "tga2d/render/RenderCommon.h"
#include "tga2d/model/AnimatedModelInstance.h"

namespace Tga2D
{
	class ModelInstancer;
	class Texture;
}

class GameWorld
{
	

public:
	GameWorld(); 
	~GameWorld();

	void Init();
	void Update(float aTimeDelta); 
	void Render();
private:
	

	//Tga2D::AnimatedModelInstance model;
};