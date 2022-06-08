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
{
}

GameWorld::~GameWorld()
{
}

void GameWorld::Init()
{
	myRoot = std::make_shared<QuadNode<GameObject>>(Tga2D::Vector2f{ 0.5f, 0.5f }, Tga2D::Vector2f{ 1.f,1.f });

	Tga2D::Engine::GetInstance()->SetTargetSize({ 800, 800 });
	Tga2D::Engine::GetInstance()->SetResolution({ 800, 800 });
	Tga2D::Engine::GetInstance()->SetClearColor({ 0.f, 0.f, 0.f, 0.f });
}
void GameWorld::Update(float aTimeDelta)
{
	using CommonUtilities::Mouse;
	aTimeDelta;
	//model.Update(aTimeDelta); // model.Update(1.0f / 60.0f);



	if (Mouse::GetButtonDown(Mouse::Key::LeftMouseButton))
	{
		GameObject newObj = GameObject();
		newObj.myPosition = GetLinearCoords(Mouse::GetMousePosition());
		newObj.mySize = GetLinearCoords(CommonUtilities::Vector2<int>{ 10, 10 });
		myRoot->Insert(newObj);
	}
}

void GameWorld::Render()
{
	if (myRoot)
		myRoot->Render();
}


Tga2D::Vector2f GameWorld::GetLinearCoords(CommonUtilities::Vector2<int> someCoords)
{
	auto targetRez = Tga2D::Engine::GetInstance()->GetTargetSize();

	return { static_cast<float>(someCoords.x) / static_cast<float>(targetRez.x), static_cast<float>(someCoords.y) / static_cast<float>(targetRez.y) };
}
