#pragma once
#include <vector>
#include <array>
#include <memory>
#include <tga2d/math/Transform.h>

template<typename Object>
class QuadNode
{
public:
	QuadNode(const Tga2D::Vector2f aPos, const Tga2D::Vector2f aSize);
	void Insert(Object& anObjectToAdd);
private:
	bool myHasChildrenFlag;

	Tga2D::Vector2f mySize;
	Tga2D::Vector2f myPosition;

	std::vector<std::shared_ptr<QuadNode>> myChildNodes;
	std::vector< Object> myContents;

	void InsertInChildren(Object& anObjectToAdd);
	void SplitNode();


};

template< typename Object>
inline QuadNode<Object>::QuadNode(const Tga2D::Vector2f aPos, const Tga2D::Vector2f aSize)
{
	mySize = aSize;
	myPosition = aPos;
	myHasChildrenFlag = false;
}

template<typename Object>
inline void QuadNode<Object>::Insert(Object& anObjectToAdd)
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
inline void QuadNode<Object>::InsertInChildren(Object& anObjectToAdd)
{
	for (auto& node : myChildNodes)
	{
		

		bool isInsideX = std::abs(anObjectToAdd.myPosition.x - node->myPosition.x) < node->mySize.x + anObjectToAdd.mySize.x;
		bool isInsideY = std::abs(anObjectToAdd.myPosition.y - node->myPosition.y) < node->mySize.y + anObjectToAdd.mySize.y;

		if (isInsideX && isInsideY)
		{
			node->Insert(anObjectToAdd);
			break;
		}
	}
}

template<typename Object>
inline void QuadNode<Object>::SplitNode()
{
	Tga2D::Vector2f halfSize = { mySize.x / 2.f, mySize.y / 2.f };

	for (size_t x = -1; x < 1; ++++x)
	{
		for (size_t y = -1; y < 1; ++++y)
		{
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

			myChildNodes.push_back(newQuad);

		}
	}
	myHasChildrenFlag = true;
	myContents.clear();
}
