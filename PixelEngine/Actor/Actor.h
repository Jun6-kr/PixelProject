#pragma once
#include "ActorType.h"
#include <Core/Core.h>
#include <Core/CraftObject.h>
#include <Math/Vector2.h>
#include <Math/Rect.h>

#include <memory>		// std::weak_ptr 사용을 위해

namespace Pixel
{
	// 전방 선언
	class Level;

	class PIXEL_API Actor : public PixelObject
	{
		TYPE_DECLARATIONS(Actor, PixelObject)

	public:
		Actor(const Vector2& position = Vector2::Zero);
		virtual ~Actor();

		// 게임 플레이 이벤트 함수
		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw(const Vector2& camStart = Vector2::Zero, const Vector2& camEnd = Vector2::Zero);

		// 충돌 이벤트 함수
		virtual void OnCollision(const std::shared_ptr<Actor>& other);

		// 액터 제거 함수
		void Destroy();

		// 게임(엔진) 종료 함수
		void QuitGame();


		// Getter/Setter
		inline bool HasBeganPlay() const { return hasBeganPlay; }
		inline bool IsActive() const { return isActive && !hasExpired; }
		inline bool HasExpired() const { return hasExpired; }

		// 
		inline std::shared_ptr<Level> GetOwner() const { return owner.lock(); }
		inline void SetOwner(std::weak_ptr<Level> newOwner) { owner = newOwner; }

		inline Vector2 GetPosition() const { return position; }
		virtual void SetPosition(const Vector2& newPosition);

		inline Vector2 GetPreviousPosition() const { return previousPosition; }
		inline void SavePreviousState() { previousPosition = position; }

		Pixel::Rect virtual GetBounds() const;
		inline ActorType GetActorType() const { return actorType; }

	protected:
		// BeginPlay 이벤트 처리 여부 플래그
		bool hasBeganPlay = false;

		// 액터 활성화 여부 플래그
		bool isActive = true;

		// 삭제 요청 여부 플래그
		bool hasExpired = false;

		// 오너십 - 이 액터를 소유하는 레벨 객체
		// weak_ptr (약한 참조) : 실제 사용을 위해서는 해당 위치가 유효한지 확인해야함
		std::weak_ptr<Level> owner;

		// 위치
		Vector2 position;

		// 이전 프레임 위치
		Vector2 previousPosition;

		ActorType actorType = ActorType::None;
	};
}

