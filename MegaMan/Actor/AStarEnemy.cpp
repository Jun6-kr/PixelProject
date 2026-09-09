#include "AStarEnemy.h"
#include <Game/GameManager.h>
#include <Level/GameLevel.h>
#include <Render/AnimationClip.h>

using namespace Pixel;

AStarEnemy::AStarEnemy(const Vector2& position) : Enemy(position)
{
	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	idleAnimtaion = game.GetClip("AStarEnemy");

	hp = 10;
}

Rect AStarEnemy::GetBounds() const
{
	return Rect(static_cast<float>(position.x), static_cast<float>(position.y), 16.0f, 8.0f);
}

void AStarEnemy::OnDamaged(const int damage)
{
	if (hp <= damage)
	{     
		hp = 0;
		Destroy();
		return;
	}

	super::OnDamaged(damage);
}

void AStarEnemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (shootTimer <= 0.0f)
	{
		shootTimer = shootInterval;
		ShootAStar();
	}
	else
	{
		shootTimer -= deltaTime;
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
		level->ShootProjectile(ProjectileType::Astar, position + Vector2(7, -5), Vector2(0, -1));
	}
}
