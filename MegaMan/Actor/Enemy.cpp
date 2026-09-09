#include "Enemy.h"

using namespace Pixel;

Enemy::Enemy(const Vector2& position) : Actor(position)
{
	actorType = ActorType::Enemy;
	blockMask = 0b10101;
	damageMask = 0b00001;
}
