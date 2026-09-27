#include "HornPlatform.h"
#include <Game/GameManager.h>
#include <Render/AnimationClip.h>

using namespace Pixel;

HornPlatform::HornPlatform(const Vector2& position) : Actor(position)
{
	actorType = ActorType::Terrain;

	blockMask = 0b11011;
	damageMask = 0;

	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	idleAnimtaion = game.GetClip("HornPlatform");
	 
	width = idleAnimtaion->GetWidth();
	height = idleAnimtaion->GetHeight();
}

void HornPlatform::Draw(const Pixel::Vector2& camStart, const Pixel::Vector2& camEnd)
{
	Vector2 pos = position - camStart;
	idleAnimtaion->PlayAnimation(0, pos, camEnd);
}
