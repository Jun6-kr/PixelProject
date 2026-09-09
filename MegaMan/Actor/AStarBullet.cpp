#include "AStarBullet.h"
#include <Level/GameLevel.h>
#include <Game/GameManager.h>
#include <Actor/Player.h>
#include <System/DebugSystem.h>

using namespace Pixel;

AStarBullet::AStarBullet()
{
}

void AStarBullet::OnSpawn(GameLevel* level)
{
	if (!level)
	{
		return;
	}

	aStar.GetScreenWorldPosition(Vector2(level->GetScreenOffsetX(), level->GetScreenOffsetY())
		, level->GetScreenWidth()
		, level->GetScreenHeight());

	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	Player* player = game.GetPlayer().lock().get();
	route = aStar.FindPath(position, player->GetPosition(), level->GetBlocked());
}

void AStarBullet::Tick(float deltaTime)
{
	if (!isActive || route.empty())
	{
		return;
	}

	moveTimer += projectileDef.speed * deltaTime;
		
	while (moveTimer >= 1.0f && astarIndex < route.size() - 1)
	{
		moveTimer -= 1.0f;
		++astarIndex;

		posX = static_cast<float>(route[astarIndex].x);
		posY = static_cast<float>(route[astarIndex].y);

		std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner());

		CollisionResult result = level->CheckCollision(this, GetBounds());
		if (result.isDamaged)
		{
			astarIndex = 0;
			result.actor.lock()->OnDamaged(projectileDef.damage);
			SetActive(false);
			return;
		}

		Vector2 newPosition = GetPosition();
		newPosition.x = route[astarIndex].x;
		newPosition.y = route[astarIndex].y;
		SetPosition(newPosition);
	}

	if (astarIndex >= route.size() - 1)
	{
		astarIndex = 0;
		SetActive(false);

		return;
	}
}

void AStarBullet::Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd)
{
	super::Draw(screenOffset, screenEnd);

	for (int ix = 0; ix < route.size(); ++ix)
	{
		JxDebug::DebugSystem::Get().AddBox(route[ix].x - screenOffset.x, route[ix].y - screenOffset.y, 1, 1, (ix < astarIndex) ? 0x0E : 0x0D);
	}
}

void AStarBullet::OnCollision(const Actor* other)
{
	astarIndex = 0;
	super::OnCollision(other);
}
