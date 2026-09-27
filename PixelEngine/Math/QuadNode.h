#pragma once
#include "Math/Rect.h"

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

	// 영역 구분을 위한 열거형
	enum class NodeIndex
	{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,

		Straddling,		// 경계선에 걸치는 경우
		OutOfArea		// 영역을 벗어난 경우
	};

	class QuadNode
	{
	public:
		QuadNode(const Rect& bounds, int depth = 0);
		~QuadNode();

		bool Insert(Actor* newActor);

		void Query(const Rect& queryBounds, std::vector<Actor*>& result) const;

		void Clear();

		const Rect& GetBounds() const { return bounds; }
		const std::vector<Actor*>& GetContainedActors() const { return containedActors; }

		QuadNode* GetTopLeft() const { return topLeft; }
		QuadNode* GetTopRight() const { return topRight; }
		QuadNode* GetBottomLeft() const { return bottomLeft; }
		QuadNode* GetBottomRight() const { return bottomRight; }

		void GetNodeRect(std::vector<Rect>& rect);

	private:
		// 영역을 4분할 할 때 사용할 함수 - 최대 깊이에 도달하면 분할 실패
		bool Subdivide();

		// 영역이 분할됐는지 여부를 확인할 때 사용할 함수
		bool IsDivided() const;

		// 전달한 영역과 겹치는 영역을 반환하는 함수
		NodeIndex TestRegion(const Rect& targetBounds) const;

		// 전달한 영역이 포함되거나 걸치는 영역을 모두 구할 때 사용할 함수
		std::vector<NodeIndex> GetQuads(const Rect& targetBounds) const;

		// 자식 노드 정리하는 함수
		void ClearChildren();

	private:
		int currentDepth = 0;

		Rect bounds;
		std::vector<Actor*> containedActors;

		QuadNode* topLeft = nullptr;
		QuadNode* topRight = nullptr;
		QuadNode* bottomLeft = nullptr;
		QuadNode* bottomRight = nullptr;
	};
}

