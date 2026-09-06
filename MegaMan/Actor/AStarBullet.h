#pragma once
#include "Projectile.h"
#include <Math/AstarAlgorithm.h>

class AStarBullet : public Projectile
{
	TYPE_DECLARATIONS(AStarBullet, Projectile)

public:
	AStarBullet();

private:
	// 이동 방향과 해당 방향의 이동 비용
	struct Direction
	{
		int x = 0;
		int y = 0;
		float cost = 0.0f;
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

	// 메시지 = 공개 메소드
	std::vector<Pixel::Vector2> FindPath(const Pixel::Vector2& startPosition, const Pixel::Vector2& goalPosition, std::vector<std::vector<int>>& grid);

	// 탐색한 최종 경로를 출력하는 함수
	void DisplayGridWithPath(std::vector<std::vector<int>>& grid, const std::vector<Pixel::Vector2>& path);

private:
	// a
	// 객체가 가진 공개되지 않은 기능? -> 메소드

	// 이전 탐색에 사용한 정보 및 노드를 정리하는 함수
	void Clear();

	// 노드 생성 전문 함수
	Pixel::Node* CreateNode(const Pixel::Vector2& position, Pixel::Node* parent = nullptr);

	// 부모 노드를 역추적해서 경로를 구성하는 함수
	std::vector<Pixel::Vector2> ConstructPath(Pixel::Node* destination);

	// 휴리스틱(추정 비용) 계산 함수
	float CalculateHeuristic(const Pixel::Vector2& current, const Pixel::Vector2& goal) const;

	// 그리드 검증 함수
	bool IsValidGrid(const std::vector<std::vector<int>>& grid) const;

	// 이동하려는 좌표가 유효한지 확인
	bool IsInRange(int x, int y, const std::vector<std::vector<int>>& grid) const;

	// 옵션 :  대각선 이동이 장애물 모서리를 통과하는지 확인
	bool IsDiagonalBlocked(const Pixel::Vector2& current, const Direction& direction, const std::vector<std::vector<int>>& grid) const;

	// Open / Close 목록 검사 함수
	Pixel::Node* FindOpenNode(int x, int y) const;
	bool IsInClosedList(int x, int y) const;

	// 목적지 인지 확인하는 함수
	bool IsDestination(const Pixel::Node* node) const;

	// 이전 탐색 표시를 지우고 그리드를 출력하는 함수
	void ClearVisualization(std::vector<std::vector<int>>& grid) const;
	void DisplayGrid(std::vector<std::vector<int>>& grid) const;

private:
	// 동적 할당된 모든 노드를 소유하는 목록 -> 소멸자에서 전체 동적할당 노드 메모리 해제시 사용
	std::vector<Pixel::Node*> allocatedNodes;

	// 탐색할 노드 목록
	std::vector<Pixel::Node*> openList;
	// //탐색을 마친 노드 목록
	std::vector<Pixel::Node*> closedList;

	// 시작/목표 노드
	Pixel::Node* startNode = nullptr;
	Pixel::Node* goalNode = nullptr;
};

