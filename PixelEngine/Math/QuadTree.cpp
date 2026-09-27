#include "QuadTree.h"
#include "QuadNode.h"
#include <Actor/Actor.h>


namespace Pixel
{
	QuadTree::QuadTree()
	{
	}

	QuadTree::~QuadTree()
	{
		SafeDelete(root);
	}

	void QuadTree::ResetTree(const Rect& screenRect)
	{
		SafeDelete(root);
		root = new QuadNode(screenRect);
	}

	bool QuadTree::Insert(Actor* actor)
	{
		if (!actor)
		{
			return false;
		}

		return root->Insert(actor);
	}

	std::vector<Actor*> QuadTree::Query(const Rect& queryRect)
	{
		// 노드의 영역과 겹치는 노드 검색
		std::vector<Actor*> intersects;
		root->Query(queryRect, intersects);

		// 검사 결과 반환
		return intersects;
	}

	std::vector<Rect>& QuadTree::GetDebugRect()
	{
		debugRect.clear();
		
		if (root)
		{
			root->GetNodeRect(debugRect);
		}

		return debugRect;
	}
}