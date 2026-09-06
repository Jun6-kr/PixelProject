#pragma once
#include "Math/Vector2.h"
#include <Core/Core.h>

namespace Pixel
{
	class PIXEL_API Node
	{
	public:
		Node(const Vector2& position, Node* parent = nullptr) : position(position), parent(parent)
		{}

	public:
		// 노드의 위치 정보
		Vector2 position;

		// 노드의 비용 정보
		float gCost = 0.0f;		// 현 노드까지 누적 이동 비용 
		float hCost = 0.0f;		// 휴리스틱, 도착지 까지의 예상 비용
		float fCost = 0.0f;

		// 최종 경로를 역추적할 때 사용할 부모 노드
		Node* parent = nullptr;
	};
}