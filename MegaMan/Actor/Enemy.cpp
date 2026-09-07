#include "Enemy.h"

using namespace Pixel;

Enemy::Enemy(const Vector2& position) : Actor(position)
{
	actorType = ActorType::Enemy;
	blockMask = 0b01001;
	damageMask = 0b00010;
}

Rect Enemy::GetBounds() const
{
    return Rect();
}
