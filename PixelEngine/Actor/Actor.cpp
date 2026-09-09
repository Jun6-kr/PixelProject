#include "Actor.h"
#include <Engine/Engine.h>

namespace Pixel
{
	Actor::Actor(const Vector2& position)
		: position(position)
	{
	}

	Actor::~Actor()
	{
	}

	// 게임 플레이 이벤트 함수
	void Actor::BeginPlay()
	{
		// 이벤트 처리했다고 설정.
		hasBeganPlay = true;
	}

	void Actor::Tick(float deltaTime)
	{
	}

	void Actor::Draw(const Vector2& camStart, const Vector2& camEnd)
	{
	}

	void Actor::OnCollision(const Actor* other)
	{
	}

	void Actor::Destroy()
	{
		// 삭제 예약 설정
		hasExpired = true;
	}

	void Actor::QuitGame()
	{
		// 엔진 종료 요창
		Engine::Get().Quit();
	}

	void Actor::SetPosition(const Vector2& newPosition)
	{
		// 변경하려는 위치 값이 기본 값과 동일하면 종료
		if (position == newPosition)
		{
			return;
		}

		position = newPosition;
	}

	Rect Actor::GetBounds() const
	{
		return Rect(static_cast<float>(position.x)
			, static_cast<float>(position.y)
			, static_cast<float>(width)
			, static_cast<float>(height));
	}

	void Actor::OnDamaged(const int damage)
	{
		hp -= damage;
	}
}