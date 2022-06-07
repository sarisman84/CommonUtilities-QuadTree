#pragma once
#include <vector>
#include <array>
#include <memory>
#include <tga2d/math/Transform.h>

#include <tga2d/drawers/DebugDrawer.h>

template<typename Object>
class QuadNode
{
public:
	QuadNode(const Tga2D::Vector2f aPos, const Tga2D::Vector2f aSize);
	void Insert(const Object& anObjectToAdd);
	void Render();

private:
	size_t myCurrentDepth;
	bool myHasChildrenFlag;

	Tga2D::Vector2f mySize;
	Tga2D::Vector2f myPosition;

	std::vector<std::shared_ptr<QuadNode>> myChildNodes;
	std::vector< Object> myContents;

	void InsertInChildren(const Object& anObjectToAdd);
	void SplitNode();


};

template< typename Object>
inline QuadNode<Object>::QuadNode(const Tga2D::Vector2f aPos, const Tga2D::Vector2f aSize)
{
	mySize = aSize;
	myPosition = aPos;
	myHasChildrenFlag = false;
	myCurrentDepth = 0;
}

template<typename Object>
inline void QuadNode<Object>::Insert(const Object& anObjectToAdd)
{
	if (myContents.size() >= 5)
	{
		SplitNode();
	}

	if (myHasChildrenFlag)
	{
		InsertInChildren(anObjectToAdd);
	}
	else
	{
		myContents.push_back(anObjectToAdd);
	}
}

template<typename Object>
inline void QuadNode<Object>::Render()
{
	if (myHasChildrenFlag)
	{
		for (std::shared_ptr<QuadNode<Object>>& node : myChildNodes)
		{
			node->Render();
		}
		return;
	}

	Tga2D::DebugDrawer& drawer = Tga2D::Engine::GetInstance()->GetDebugDrawer();


	std::array<Tga2D::Vector2f, 2> myStartPoint, myEndPoint;

	myStartPoint[0] = { myPosition.x - mySize.x, myPosition.y - mySize.y };
	myEndPoint[0] = { myPosition.x + mySize.x , myPosition.y - mySize.y };

	myStartPoint[1] = { myPosition.x - mySize.x, myPosition.y - mySize.y };
	myEndPoint[1] = { myPosition.x - mySize.x , myPosition.y + mySize.y };




	std::array<Tga2D::Color, 2> lineColor;
	lineColor[0] = { 0.25f,1,0.25f,1 };
	lineColor[1] = { 1.0f,0.25f,0.25f,1 };
	drawer.DrawLines(myStartPoint.data(), myEndPoint.data(), lineColor.data(), 2);

	for (auto& obj : myContents)
	{
		drawer.DrawCircle(obj.myPosition, obj.mySize.x);
	}


}

template<typename Object>
inline void QuadNode<Object>::InsertInChildren(const Object& anObjectToAdd)
{
	for (auto& node : myChildNodes)
	{
		bool isInsideX = std::abs(anObjectToAdd.myPosition.x - node->myPosition.x) < node->mySize.x + anObjectToAdd.mySize.x;
		bool isInsideY = std::abs(anObjectToAdd.myPosition.y - node->myPosition.y) < node->mySize.y + anObjectToAdd.mySize.y;

		if (isInsideX && isInsideY)
		{
			node->Insert(anObjectToAdd);
		}
	}
}

template<typename Object>
inline void QuadNode<Object>::SplitNode()
{
	if (myCurrentDepth >= 10) return;
	Tga2D::Vector2f halfSize = { mySize.x / 2.f, mySize.y / 2.f };
	myCurrentDepth++;
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			if (x == 0 || y == 0) continue;

			Tga2D::Vector2f newPos = myPosition + Tga2D::Vector2f(halfSize.x * x, halfSize.y * y);
			auto newQuad = std::make_shared<QuadNode<Object>>(newPos, halfSize);

			for (size_t objIndex = 0; objIndex < myContents.size(); objIndex++)
			{
				bool isInsideX = std::abs(myContents[objIndex].myPosition.x - newPos.x) < halfSize.x + myContents[objIndex].mySize.x;
				bool isInsideY = std::abs(myContents[objIndex].myPosition.y - newPos.y) < halfSize.y + myContents[objIndex].mySize.y;

				if (isInsideX && isInsideY)
				{
					newQuad->Insert(myContents[objIndex]);
				}

			}
			newQuad->myCurrentDepth = myCurrentDepth;
			myChildNodes.push_back(newQuad);

		}
	}
	myHasChildrenFlag = true;
	myContents.clear();
}
