#include "AStarEnemy.h"
#include <Render/AnimationClip.h>

using namespace Pixel;

AStarEnemy::AStarEnemy()
{}

void AStarEnemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void AStarEnemy::Draw(const Vector2& screenOffset, const Vector2& screenEnd)
{
	Vector2 pos = position - screenOffset;

	idleAnimtaion->PlayAnimation(0, pos, screenEnd);
}
