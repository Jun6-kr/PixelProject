#pragma once
#include "Math/Rect.h"
#include <Core/Core.h>
#include <vector>

namespace Pixel
{
	class Actor;
	class QuadNode;

	class PIXEL_API QuadTree
	{
	public:
		QuadTree();
		~QuadTree();

		void ResetTree(const Rect& screenRect);

		// 트리에 노드를 추가하는 함수
		bool Insert(Actor* actor);

		// 전달한 노드와 겹치는 노드를 반환하는 함수
		std::vector<Actor*> Query(const Rect& queryRect);

		std::vector<Rect>& GetDebugRect();

	public:
		static const int maxDepth = 5;

	private:
		std::vector<Rect> debugRect;
		QuadNode* root = nullptr;
	};
}

