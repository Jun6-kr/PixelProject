#include "Door.h"
#include <Render/AnimationClip.h>
#include <Game/GameManager.h>
#include <Level/GameLevel.h>

using namespace Pixel;

Door::Door(const Vector2& position, const int tileCountY, bool isBossRoomDoor)
	: Actor(position), tileCountY(tileCountY), isBossDoor(isBossRoomDoor)
{
	actorType = ActorType::Terrain;

	blockMask = 0b10001;
	damageMask = 0;

	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	idleAnimtaion = game.GetClip("Door");

	width = idleAnimtaion->GetWidth();
	height = idleAnimtaion->GetHeight() * tileCountY;
}

void Door::OnCollision(const Actor* other)
{
	if (other->GetActorType() != ActorType::Player)
	{
		return;
	}

	if (std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner()))
	{
		if (hasOpend)
		{
			return;
		}

		hasOpend = true;
		if (isBossDoor)
		{
			level->EnterBossRoom();
		}
		else
		{
			level->EnterBossEntryRoom();
		}

		PlayCloseAnimation(false);
	}
}

void Door::Draw(const Vector2& screenOffset, const Vector2& screenEnd)
{
	Vector2 pos = position - screenOffset;

	for (int ix = 0; ix < tileCountY; ++ix)
	{
		idleAnimtaion->PlayAnimation(0, pos, screenEnd);
		pos.y += idleAnimtaion->GetHeight();
	}
}

void Door::PlayCloseAnimation(bool isBackward)
{
}
