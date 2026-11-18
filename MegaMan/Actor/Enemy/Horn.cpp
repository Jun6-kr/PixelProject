#include "Horn.h"
#include <Game/GameManager.h>
#include <Level/GameLevel.h>
#include <Render/AnimationClip.h>

using namespace Pixel;

Horn::Horn(const Vector2& position) : Enemy(position), basePosition(position)
{
	actorType = ActorType::Enemy;

	blockMask = 0b00001;
	damageMask = 0b10000;

	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	idleAnimtaion = game.GetClip("Horn");

	size = idleAnimtaion->GetSize();
	width = idleAnimtaion->GetWidth();

	CalculateHeight();
}

void Horn::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (movingTimer <= 0.0f)
	{
		animIndex += movingDirection;

		if (animIndex < 0 || animIndex == size)
		{
			movingDirection *= -1;
			// 방향이 양수로 전환 -> 다시 처음 상태로 복귀했다는 뜻이므로 index = 0
			animIndex = movingDirection > 0 ? 0 : size - 1;
		}

		CalculateHeight();

		movingTimer = movingInterval;
	}
	else
	{
		movingTimer -= deltaTime;
	}

	std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner());
	Rect rect = GetBounds();
	CollisionResult result = level->CheckCollision(this, rect);
	
	if (result.isDamaged)
	{
		result.actor->OnDamaged(damage);
	}
}

void Horn::Draw(const Vector2& camStart, const Vector2& camEnd)
{
	Vector2 pos = position - camStart;
	idleAnimtaion->PlayAnimation(animIndex, pos, camEnd);
}

void Horn::CalculateHeight()
{
	height = animIndex + 1;
	int heightStart = idleAnimtaion->GetSize() - height;
	position.y = basePosition.y + heightStart;
}
