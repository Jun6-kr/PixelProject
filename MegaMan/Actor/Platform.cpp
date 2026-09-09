#include "Platform.h"
#include <Game/GameManager.h>
#include <Render/AnimationClip.h>

using namespace Pixel;

Platform::Platform(const std::string& id, const Vector2& position, int tileCountX, int tileCountY) : Actor(position), tileCountX(tileCountX), tileCountY(tileCountY)
{
	actorType = ActorType::Terrain;
	blockMask = 0b11011;
	damageMask = 0b00000;

	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	idleAnimtaion = game.GetClip(id);

	width = idleAnimtaion->GetWidth() * tileCountX;
	height = idleAnimtaion->GetHeight() * tileCountY;
}

Platform::~Platform()
{
}

void Platform::Draw(const Vector2& camOffset, const Vector2& camEnd)
{
	Vector2 pos = position - camOffset;

	int tileWidth = idleAnimtaion->GetWidth();
	int tileHeight = idleAnimtaion->GetHeight();

	for (int iy = 0; iy < tileCountY; ++iy)
	{
		for (int ix = 0; ix < tileCountX; ++ix)
		{
			Vector2 drawPos;

			drawPos.x = pos.x + ix * tileWidth;
			drawPos.y = pos.y + iy * tileHeight;

			idleAnimtaion->PlayAnimation(0, drawPos, camEnd);
		}
	}
}
