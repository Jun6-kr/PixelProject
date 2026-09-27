#include "QuadNode.h"
#include "QuadTree.h"
#include <Actor/Actor.h>

#include <System/DebugSystem.h>

namespace Pixel
{
	QuadNode::QuadNode(const Rect& bounds, int depth) : bounds(bounds), currentDepth(depth)
	{
	}
	QuadNode::~QuadNode()
	{
		Clear();
	}

	bool QuadNode::Insert(Actor * newActor)
	{
		if (!newActor)
		{
			return false;
		}

		if (!bounds.Intersects(newActor->GetBounds()))
		{
			return false;
		}

		NodeIndex result = TestRegion(newActor->GetBounds());

		// 분할이 더 이상 안 되거나, 경계에 걸쳐있다면 현재 노드에 추가
		if (result == NodeIndex::Straddling || !Subdivide())
		{
			containedActors.emplace_back(newActor);
			return true;
		}

		// 완전히 포함되는 자식 노드에 삽입
		if (result == NodeIndex::TopLeft)
		{
			return topLeft->Insert(newActor);
		}
		if (result == NodeIndex::TopRight)
		{
			return topRight->Insert(newActor);
		}
		if (result == NodeIndex::BottomLeft)
		{
			return bottomLeft->Insert(newActor);
		}
		if (result == NodeIndex::BottomRight)
		{
			return bottomRight->Insert(newActor);
		}

		return false;
	}

	void QuadNode::Query(const Rect& queryBounds, std::vector<Actor*>& result) const
	{
		// 검사 영역과 겹치지 않으면 더 이상 검사 안함
		if (!bounds.Intersects(queryBounds))
		{
			return;
		}

		// 현재 노드에 저장된 객체를 실제 영역과 비교
		for (Actor* const point : containedActors)
		{
			if (point && point->GetBounds().Intersects(queryBounds))
			{
				result.emplace_back(point);
			}
		}

		// 분할된 경우라면 자식 노드까지 검사
		if (IsDivided())
		{
			topLeft->Query(queryBounds, result);
			topRight->Query(queryBounds, result);
			bottomLeft->Query(queryBounds, result);
			bottomRight->Query(queryBounds, result);
		}
	}

	void QuadNode::Clear()
	{
		// Points에 추가된 노드는 외부에 존재하는 노드
		// 따라서 이 범위에서 메모리 관리하지 않는다

		// 리스트 정리
		containedActors.clear();

		// 자식 노드는 직접 관리
		ClearChildren();
	}

	void QuadNode::GetNodeRect(std::vector<Rect>& rect)
	{
		rect.emplace_back(bounds);

		if (IsDivided())
		{
			topLeft->GetNodeRect(rect);
			topRight->GetNodeRect(rect);
			bottomLeft->GetNodeRect(rect);
			bottomRight->GetNodeRect(rect);
		}
	}

	bool QuadNode::Subdivide()
	{
		// 최대 깊이 확인
		if (currentDepth >= QuadTree::maxDepth)
		{
			return false;
		}

		// 분할됐으면 반환
		if (IsDivided())
		{
			return true;
		}

		// 영역을 나누기 위한 갑
		float x = static_cast<float>(bounds.GetX());
		float y = static_cast<float>(bounds.GetY());
		float halfWidth = static_cast<float>(bounds.GetWidth() / 2);
		float halfHeight =static_cast<float>(bounds.GetHeight() / 2);

		// 4분할을 담당하는 객체 생성
		topLeft = new QuadNode(Rect(x, y, halfWidth, halfHeight), currentDepth + 1);
		topRight = new QuadNode(Rect(x + halfWidth, y, halfWidth, halfHeight), currentDepth + 1);
		bottomLeft = new QuadNode(Rect(x, y + halfHeight, halfWidth, halfHeight), currentDepth + 1);
		bottomRight = new QuadNode(Rect(x + halfWidth, y + halfHeight, halfWidth, halfHeight), currentDepth + 1);

		return true;
	}

	bool QuadNode::IsDivided() const
	{
		// 자손 노드 4개 중 하나라도 null이 아니면 분할됐다고 판단
		return topLeft != nullptr;
	}

	NodeIndex QuadNode::TestRegion(const Rect& targetBounds) const
	{
		// 현재 노드의 영역을 벗어난 경우 확인
		if (!bounds.Intersects(targetBounds))
		{
			return NodeIndex::OutOfArea;
		}

		// 전달 된 영역이 어느 사분면에 포함되는 지 확인
		std::vector<NodeIndex> quads = GetQuads(targetBounds);

		// 반환 받은 배열의 요소 개수가 1개라면 4분면 중 1개 영역에 완전 포함
		if (quads.size() == 1)
		{
			return quads[0];
		}

		// 여러 영역에 겹쳤다고 반환
		return NodeIndex::Straddling;
	}

	std::vector<NodeIndex> QuadNode::GetQuads(const Rect& targetBounds) const
	{
		// 겹치는 영역을 포함한 배열
		std::vector<NodeIndex> quads;

		// 영역 계산에 필요한 변수
		int x = bounds.GetX();
		int y = bounds.GetY();

		int halfWidth = bounds.GetWidth() / 2;
		int halfHeight = bounds.GetHeight() / 2;
		int centerX = x + halfWidth;
		int centerY = y + halfHeight;

		// 왼쪽 영역에 완전히 포핟되는 지 확인
		bool left = targetBounds.GetX() >= x && targetBounds.GetXMax() <= centerX;

		// 오른쪽 영역에 완전히 포함되는 지 확인
		bool right = targetBounds.GetX() >= centerX && targetBounds.GetXMax();

		// 위쪽 영역에 완전히 포함되는지 확인.
		bool top = targetBounds.GetY() >= y && targetBounds.GetYMax() <= centerY;

		// 아래쪽 영역에 완전히 포함되는지 확인.
		bool bottom = targetBounds.GetY() >= centerY
			&& targetBounds.GetYMax() <= bounds.GetYMax();

		if (top && left)
		{
			quads.emplace_back(NodeIndex::TopLeft);
		}
		if (top && right)
		{
			quads.emplace_back(NodeIndex::TopRight);
		}
		if (bottom && left)
		{
			quads.emplace_back(NodeIndex::BottomLeft);
		}
		if (bottom && right)
		{
			quads.emplace_back(NodeIndex::BottomRight);
		}

		return quads;
	}

	void QuadNode::ClearChildren()
	{
		if (IsDivided())
		{
			SafeDelete(topLeft);
			SafeDelete(topRight);
			SafeDelete(bottomLeft);
			SafeDelete(bottomRight);
		}
	}
}