#include "AStarEnemy.h"
#include <Game/GameManager.h>
#include <Level/GameLevel.h>
#include <Render/AnimationClip.h>

#include <Input/Input.h>

using namespace Pixel;

AStarEnemy::AStarEnemy(const Vector2& position) : Enemy(position)
{
	actorType = ActorType::Enemy;
	blockMask = 0b01001;
	damageMask = 0b00010;

	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	idleAnimtaion = game.GetClip("AStarEnemy");
}

Rect AStarEnemy::GetBounds() const
{
	return Rect(static_cast<float>(position.x), static_cast<float>(position.y), 16.0f, 8.0f);
}

void AStarEnemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (Input::Get().GetKeyDown('T'))
	{
		ShootAStar();
	}

}

void AStarEnemy::Draw(const Vector2& screenOffset, const Vector2& screenEnd)
{
	Vector2 pos = position - screenOffset;

	idleAnimtaion->PlayAnimation(0, pos, screenEnd);
}

void AStarEnemy::ShootAStar()
{
	if (std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner()))
	{
		level->ShootProjectile(ProjectileType::Astar, position + Vector2(7, 0), Vector2(0, -1));
	}
}
