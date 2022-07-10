#pragma once
#include <vector>
#include <array>
#include <memory>
#include <functional>
#include <tga2d/math/Transform.h>
#include <algorithm>
#include <tga2d/drawers/DebugDrawer.h>
#include "../External/Custom/Input.h"

template<typename Object>
class QuadNode
{
public:
	QuadNode(const Tga2D::Vector2f& aPos, const Tga2D::Vector2f& aSize);
	void Insert(const Object& anObjectToAdd);
	void Render();

private:



	size_t myCurrentDepth;
	bool myHasChildrenFlag;

	Tga2D::Vector2f mySize;
	Tga2D::Vector2f myPosition;

	std::vector<std::shared_ptr<QuadNode>> myChildNodes;
	std::vector< Object> myContents;

	void InsertInChildren(const Object& anObjectToAdd, std::function<void(Object& anObject)> anInsertCallack = nullptr);
	void MoveContentsToChildren();
	void SplitNode();

	const bool CheckNodeAgainstPosition(const Tga2D::Vector2f aPosition);
};

template< typename Object>
inline QuadNode<Object>::QuadNode(const Tga2D::Vector2f& aPos, const Tga2D::Vector2f& aSize)
{
	mySize = aSize;
	myPosition = aPos;
	myHasChildrenFlag = false;
	myCurrentDepth = 0;
}

template<typename Object>
inline void QuadNode<Object>::Insert(const Object& anObjectToAdd)
{
	myContents.push_back(anObjectToAdd);
	if (myContents.size() >= 5 && !myHasChildrenFlag)
	{
		SplitNode();
		return;
	}

	if (myHasChildrenFlag)
	{
		InsertInChildren(anObjectToAdd);
	}

}

template<typename Object>
inline void QuadNode<Object>::Render()
{
	using namespace CommonUtilities;
	auto pixelPos = Mouse::GetMousePosition();

	auto targetRez = Tga2D::Engine::GetInstance()->GetTargetSize();


	Tga2D::Vector2f pos = Tga2D::Vector2f(static_cast<float>(pixelPos.x) / static_cast<float>(targetRez.x), static_cast<float>(pixelPos.y) / static_cast<float>(targetRez.y));
	bool isMouseInQuadNode = CheckNodeAgainstPosition(pos);


	Tga2D::DebugDrawer& drawer = Tga2D::Engine::GetInstance()->GetDebugDrawer();


	for (auto& obj : myContents)
	{

		drawer.DrawCircle(obj.myPosition, obj.mySize.x, isMouseInQuadNode ? myHasChildrenFlag ? Tga2D::Color(0, 0, 1, 1) : Tga2D::Color(0, 1, 0, 1) : Tga2D::Color(1, 1, 1, 1));
	}



	if (myHasChildrenFlag)
	{
		for (std::shared_ptr<QuadNode<Object>>& node : myChildNodes)
		{
			node->Render();
		}
		return;
	}




	std::array<Tga2D::Vector2f, 2> myStartPoint, myEndPoint;

	myStartPoint[0] = { myPosition.x - mySize.x, myPosition.y - mySize.y };
	myEndPoint[0] = { myPosition.x + mySize.x , myPosition.y - mySize.y };

	myStartPoint[1] = { myPosition.x - mySize.x, myPosition.y - mySize.y };
	myEndPoint[1] = { myPosition.x - mySize.x , myPosition.y + mySize.y };




	std::array<Tga2D::Color, 2> lineColor;
	lineColor[0] = { 0.25f,1,0.25f,1 };
	lineColor[1] = { 1.0f,0.25f,0.25f,1 };
	drawer.DrawLines(myStartPoint.data(), myEndPoint.data(), lineColor.data(), 2);




}

template<typename Object>
inline void QuadNode<Object>::InsertInChildren(const Object& anObjectToAdd, std::function<void(Object& anObject)> anInsertCallack)
{
	int targetNode = -1;
	int validAmm = 0;
	for (int n = 0; n < myChildNodes.size(); n++)
	{
		const bool isInsideX = std::abs(anObjectToAdd.myPosition.x - myChildNodes[n]->myPosition.x) < myChildNodes[n]->mySize.x + anObjectToAdd.mySize.x;
		const bool isInsideY = std::abs(anObjectToAdd.myPosition.y - myChildNodes[n]->myPosition.y) < myChildNodes[n]->mySize.y + anObjectToAdd.mySize.y;

		if (isInsideX && isInsideY)
		{
			targetNode = n;
			validAmm++;
		}
	}

	if (validAmm == 1)
	{
		myChildNodes[targetNode]->Insert(anObjectToAdd);
		if (anInsertCallack)
			anInsertCallack(const_cast<Object&>(anObjectToAdd));

	}






}

template<typename Object>
inline void QuadNode<Object>::MoveContentsToChildren()
{
	for (size_t i = 0; i < myContents.size(); i++)
	{
		InsertInChildren(myContents[i], [&](Object& anElement)
			{
				myContents.erase(std::remove(myContents.begin(), myContents.end(), anElement), myContents.end());
				i--;
			});
	}


}

template<typename Object>
inline void QuadNode<Object>::SplitNode()
{
	if (myCurrentDepth >= 10) return;
	const Tga2D::Vector2f halfSize = { mySize.x / 2.f, mySize.y / 2.f };
	myCurrentDepth++;



	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			if (x == 0 || y == 0) continue;

			const Tga2D::Vector2f newPos = myPosition + Tga2D::Vector2f(halfSize.x * x, halfSize.y * y);
			auto newQuad = std::make_shared<QuadNode<Object>>(newPos, halfSize);

			newQuad->myCurrentDepth = myCurrentDepth;
			myChildNodes.push_back(newQuad);

		}
	}
	myHasChildrenFlag = true;

	MoveContentsToChildren();

}

template<typename Object>
inline const bool QuadNode<Object>::CheckNodeAgainstPosition(const Tga2D::Vector2f aPosition)
{
	Tga2D::Vector2f min = myPosition - Tga2D::Vector2f(mySize.x, mySize.y);
	Tga2D::Vector2f max = myPosition + Tga2D::Vector2f(mySize.x, mySize.y);


	return aPosition.x >= min.x && aPosition.x <= max.x && aPosition.y >= min.y && aPosition.y <= max.y;
}
