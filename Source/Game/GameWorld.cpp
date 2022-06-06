#include "stdafx.h"
#include "GameWorld.h"
#include <tga2d/engine.h>
#include <tga2d/error/ErrorManager.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/drawers/SpriteDrawer.h>
#include <tga2d/texture/TextureManager.h>

#include "tga2d/drawers/ModelDrawer.h"
#include "tga2d/graphics/Camera.h"
#include "tga2d/graphics/DX11.h"
#include "tga2d/model/ModelFactory.h"

GameWorld::GameWorld()
{}

GameWorld::~GameWorld() 
{}

void GameWorld::Init()  
{
	
}
void GameWorld::Update(float aTimeDelta)
{
	aTimeDelta;
	//model.Update(aTimeDelta); // model.Update(1.0f / 60.0f);
}

void GameWorld::Render()
{
	
}