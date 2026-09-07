#include "Platform.h"
#include <Game/GameManager.h>
#include <Render/AnimationClip.h>

using namespace Pixel;

Platform::Platform(const Vector2& position, int tileCountX) : Actor(position), tileCountX(tileCountX)
{
	actorType = ActorType::Terrain;
	blockMask = 0b11010;
	damageMask = 0b00000;

	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	idleAnimtaion = game.GetClip("Platform");

	width = idleAnimtaion->GetWidth() * tileCountX;
	height = idleAnimtaion->GetHeight();
}

Platform::~Platform()
{
}

Rect Platform::GetBounds() const
{
	return Rect(static_cast<float>(position.x)
		, static_cast<float>(position.y)
		, static_cast<float>(width)
		, static_cast<float>(height));
}

void Platform::Draw(const Vector2& camOffset, const Vector2& camEnd)
{
	Vector2 pos = position - camOffset;

	for (int ix = 0; ix < tileCountX; ++ix)
	{
		idleAnimtaion->PlayAnimation(0, pos, camEnd);
		pos.x += idleAnimtaion->GetWidth();
	}
}
