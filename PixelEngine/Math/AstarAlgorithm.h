#pragma once
#include <Core/Core.h>
#include "Math/Vector2.h"
#include <Debug/DebugAdapter.h>
#include <vector>
#include <unordered_set>

namespace Pixel
{
	class PIXEL_API AStarNode
	{
	public:
		AStarNode(const Vector2& position, AStarNode* parent = nullptr) : position(position), parent(parent)
		{}

	public:
		// 노드의 위치 정보
		Vector2 position;

		// 노드의 비용 정보
		int gCost = 0;		// 현 노드까지 누적 이동 비용 
		int hCost = 0;		// 휴리스틱, 도착지 까지의 예상 비용
		int fCost = 0;

		// 최종 경로를 역추적할 때 사용할 부모 노드
		AStarNode* parent = nullptr;
	};

	class PIXEL_API AstarAlgorithm
	{
	private:
		// 이동 방향과 해당 방향의 이동 비용
		struct Direction
		{
			int x = 0;
			int y = 0;
			float cost = 0;
		};

		// 그리드 타일의 종류 (열거형)
		enum class TileType : char
		{
			Ground = 0,
			Wall = 1,
			Start = 2,
			Goal = 3,
			Visited = 5,
		};

	public:
		std::vector<Vector2> FindPath(const Vector2& start, const Vector2& goal, const std::unordered_set<Vector2>& blocked);

		void GetScreenWorldPosition(const Vector2& screenStartPos, int screenOffsetX, int screenOffsetY)
		{
			worldStartPosition = screenStartPos;
			worldBoundary.x = screenStartPos.x + screenOffsetX;
			worldBoundary.y = screenStartPos.y + screenOffsetY;
		}

	private:
		// 이전 탐색에 사용한 정보 및 노드를 정리하는 함수
		void Clear();

		// 노드 생성 전문 함수
		AStarNode* CreateNode(const Vector2& position, AStarNode* parent = nullptr);

		// 부모 노드를 역추적해서 경로를 구성하는 함수
		std::vector<Vector2> ConstructPath(AStarNode* destination);

		// 휴리스틱(추정 비용) 계산 함수
		int CalculateHeuristic(const Vector2& current, const Vector2& goal) const;

		// Open / Close 목록 검사 함수
		bool IsInClosedList(int x, int y) const;
		AStarNode* FindOpenNode(int x, int y) const;

		bool IsInRange(int x, int y) const;

		const int GetIndexInOpenList(const Vector2& position);

	private:
		Vector2 worldStartPosition;
		Vector2 worldBoundary;
		
		// 동적 할당된 모든 노드를 소유하는 목록 -> 소멸자에서 전체 동적할당 노드 메모리 해제시 사용
		std::vector<Pixel::AStarNode*> allocatedNodes;

		// 탐색할 노드 목록
		std::vector<Pixel::AStarNode*> openList;
		std::unordered_map<Pixel::Vector2, Pixel::AStarNode*> openMap;
		
		// //탐색을 마친 노드 목록
		std::unordered_set<Pixel::Vector2> closedSet;

		// 시작/목표 노드
		Pixel::AStarNode* startNode = nullptr;
		Pixel::AStarNode* goalNode = nullptr;

		DebugAdapter deebug;
	};
}