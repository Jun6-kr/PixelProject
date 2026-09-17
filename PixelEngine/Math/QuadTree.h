#pragma once
#include "Math/Rect.h"
#include <Core/Core.h>
#include <vector>

namespace Pixel
{
	// 메모리 삭제 함수
	template<typename T>
	void SafeDelete(T*& t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}

	class Actor;

	enum class NodeIndex
	{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,

		Straddling,		// 경계선에 걸치는 경우
		OutOfArea		// 영역을 벗어난 경우
	};

	class PIXEL_API QuadTree
	{
	public:
		QuadTree(const Rect& bounds, int depth, int maxObjectsPerNode);

		void Insert(Actor* actor);
		void Query(const Rect& queryBounds, std::vector<Actor*>& result) const;

		void Reset(const Rect& newBounds);

		void Clear();

	private:
		bool Subdivide();
		bool IsDivided() const { return topLeft != nullptr; }
		
	public:
		static const int maxDepth = 4;

	private:
		Rect bounds;
		int depth;
		int maxObjectsPerNode;

		std::vector<Actor*> objects;

		QuadTree* topLeft = nullptr;
		QuadTree* topRight = nullptr;
		QuadTree* bottomLeft = nullptr;
		QuadTree* bottomRight = nullptr;
	};
}

