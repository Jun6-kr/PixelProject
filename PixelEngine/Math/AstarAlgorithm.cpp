#include "AstarAlgorithm.h"
#include <System/DebugSystem.h>

namespace Pixel
{
	// 현재 노드를 기준(루트)으로 서브 트리를 최대(또는 최소)힙으로 만드는 함수.
	void Heapify(std::vector<Node*>& vec, int length, int index)
	{
		while (true)
		{
			// 가장 큰(또는 작은) 값을 가진 인덱스.
			int selected = index;

			// 왼쪽 자손 인덱스.
			int leftChild = index * 2 + 1;

			// 오른쪾 자손 인덱스.
			int rightChild = index * 2 + 2;

			// 왼쪽 자손이 현재 노드 보다 크면(또는 작으면) 인덱스 교환.
			//if (leftChild < length && array[leftChild] > array[selected])
			if (leftChild < length
				&& ((vec[leftChild]->fCost > vec[selected]->fCost)
				|| (vec[leftChild]->fCost == vec[selected]->fCost && vec[leftChild]->hCost < vec[selected]->hCost)))
			{
				selected = leftChild;
			}

			// 오른쪽 자손이 현재 노드 보다 크면(또는 작음) 인덱스 교환.
			//if (rightChild < length && array[rightChild] > array[selected])
			if (rightChild < length
				&& ((vec[rightChild]->fCost > vec[selected]->fCost)
				|| (vec[rightChild]->fCost == vec[selected]->fCost && vec[rightChild]->hCost < vec[selected]->hCost)))
			{
				selected = rightChild;
			}

			// 현재 노드가 컸다면, 힙이 유지됐기 때문에 종료.
			if (selected == index)
			{
				break;
			}

			// 값 교환.
			std::swap(vec[index], vec[selected]);

			// 값을 교환한 위치에서 다시 자손 노드들과 비교.
			index = selected;
		}
	}

	// 마지막 원소부터 위로 올리면서 정리
	void PushHeap(std::vector<Node*>& vec)
	{
		int index = static_cast<int>(vec.size() - 1);

		// 루트의 최대(또는 최소) 값을 배열의 뒤쪽부터 정렬.
		while(index > 0)
		{
			int parent = (index - 1) / 2;

			bool shouldSwap = (vec[index]->fCost < vec[parent]->fCost)
				|| (vec[index]->fCost == vec[parent]->fCost && vec[index]->hCost < vec[parent]->hCost);

			if (!shouldSwap)
			{
				break;
			}

			std::swap(vec[0], vec[index]);
			index = parent;
		}
	}

	std::vector<Vector2> AstarAlgorithm::FindPath(const Vector2& start, const Vector2& goal, const std::unordered_set<Vector2>& blocked)
	{
		Clear();

		if (!IsInRange(start.x, start.y) || ! IsInRange(goal.x, goal.y))
		{
			return { };
		}

		if (blocked.find(start) != blocked.end() || blocked.find(goal) != blocked.end())
		{
			return { };
		}

		// 시각화 제거

		startNode = CreateNode(start);
		goalNode = CreateNode(goal);

		startNode->gCost = 0;
		startNode->hCost = CalculateHeuristic(start, goal);
		startNode->fCost = startNode->gCost + startNode->hCost;

		openMap[startNode->position] = startNode;
		openList.emplace_back(startNode);
		PushHeap(openList);

		const std::vector<Direction> directions =
		{
			{ 0, -1, 1 }, { 0, 1, 1 }, { -1, 0, 1 }, { 1, 0, 1 },
		};

		while (!openMap.empty())
		{
			Node* currentNode = openList[0];
			 
			if (currentNode->position == goalNode->position)
			{
				return ConstructPath(currentNode);
			}

			// 힙정렬로 가장 낮은 값이 앞으로 왔고 그걸 뽑았으니까, 앞을 제거하면 되지 않을까?
			openMap.erase(currentNode->position);
			openList.erase(openList.begin());
			closedSet.insert(currentNode->position);

			for (const Direction& direction : directions)
			{
				int newX = currentNode->position.x + direction.x;
				int newY = currentNode->position.y + direction.y;

				if (!IsInRange(newX, newY))
				{
					continue;
				}

				if (blocked.find(Vector2(newX, newY)) != blocked.end())
				{
					continue;
				}

				if (IsInClosedList(newX, newY))
				{
					continue;
				}

				int newGCost = currentNode->gCost + direction.cost;

				Node* openNode = FindOpenNode(newX, newY);
				if (openNode)
				{
					if (newGCost < openNode->gCost)
					{
						openNode->gCost = newGCost;
						openNode->fCost = openNode->gCost + openNode->hCost;
						openNode->parent = currentNode;
					}

					continue;
				}

				Node* neighbor = CreateNode(Vector2(newX, newY), currentNode);

				neighbor->gCost = newGCost;
				neighbor->hCost = CalculateHeuristic(neighbor->position, goalNode->position);
				neighbor->fCost = neighbor->gCost + neighbor->hCost;
				
				openList.emplace_back(neighbor);
				PushHeap(openList);
				openMap[neighbor->position] = neighbor;
			}
		}

		return std::vector<Vector2>();
	}

	void AstarAlgorithm::Clear()
	{
		// 탐색 과정에서 생성했던 모든 노드 삭제 및 해제
		for (Node* node : allocatedNodes)
		{
			delete node;
			node = nullptr;
		}

		allocatedNodes.clear();
		openList.clear();
		openMap.clear();
		closedSet.clear();

		startNode = nullptr;
		goalNode = nullptr;
	}

	// 노드를 생성하고, allocatedNodes에 추가
	Node* AstarAlgorithm::CreateNode(const Vector2& position, Node* parent)
	{
		Node* newNode = new Node(position, parent);
		allocatedNodes.emplace_back(newNode);

		return newNode;
	}

	std::vector<Vector2> AstarAlgorithm::ConstructPath(Node* destination)
	{
		// 목표 노드로부터 부모 노드를 따라 경로 역추적
		std::vector<Vector2> path;
		Node* current = destination;

		while (current)
		{
			// 현재 노드는 경로 배열에 추가
			path.emplace_back(current->position);

			// 부모 노드로 이동해서 경로를 역추적
			current = current->parent;
		}

		// 루프가 종료되면 path에는 반대 방향의 경로 정보가 저장된다 -> 다시 역방향으로 뒤집기가 필요
		std::reverse(path.begin(), path.end());

		return path;
	}

	/// <summary>
	/// 쿼드타일(4방향) 휴리스틱 계산
	/// </summary>
	/// <param name="current">현재 조사 위치</param>
	/// <param name="goal">목표 위치</param>
	/// <returns>휴리스틱 계산값</returns>
	int AstarAlgorithm::CalculateHeuristic(const Vector2& current, const Vector2& goal) const
	{
		Vector2 pos = current - goal;
		pos.x = std::abs(pos.x);
		pos.y = std::abs(pos.y);

		return (pos.x + pos.y);
	}


	bool AstarAlgorithm::IsInClosedList(int x, int y) const
	{
		return closedSet.find(Vector2(x, y)) != closedSet.end();
	}

	Node* AstarAlgorithm::FindOpenNode(int x, int y) const
	{
		Node* node = nullptr;

		auto iter = openMap.find(Vector2(x, y));

		return (iter != openMap.end()) ? iter->second : nullptr;
	}

	bool AstarAlgorithm::IsInRange(int x, int y) const
	{
		return worldStartPosition.x <= x
			&& worldStartPosition.y <= y
			&& x < worldBoundary.x
			&& y < worldBoundary.y;
	}
}