#include "Projectile.h"
#include <Physics/Collision.h>
#include <Level/GameLevel.h>

using namespace Pixel;

Projectile::Projectile()
{
}

void Projectile::SetProjectileInfo(ProjectileDefination definition, const Pixel::Vector2& position, const Pixel::Vector2& direction)
{
	projectileDef = definition;
	SetPosition(position);
	posX = static_cast<float>(position.x);
	posY = static_cast<float>(position.y);
	this->direction = direction;
}

void Projectile::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	posX += deltaTime * projectileDef.speed * direction.x;
	posY += deltaTime * projectileDef.speed * direction.y;


	std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner());

	CollisionResult result = level->CheckCollision(this, GetBounds());
	if (result.isDamaged)
	{
		result.actor.lock()->OnDamaged(projectileDef.damage);
		SetActive(false);
		return;
	}

	Vector2 newPosition = GetPosition();
	newPosition.x = static_cast<int>(posX);
	newPosition.y = static_cast<int>(posY);

	SetPosition(newPosition);
	
}

void Projectile::Draw(const Vector2& screenOffset, const Vector2& screenEnd)
{
	if (animIndex >= projectileDef.animation->GetSize())
	{
		animIndex = 0;
	}

	Vector2 pos = position - screenOffset;

	std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner());
	if (pos.x >= level->GetScreenWidth() || pos.y >= level->GetScreenHeight() || pos.x < 0 || pos.y < 0)
	{
		SetActive(false);
		return;
	}

	projectileDef.animation->PlayAnimation(animIndex, pos, screenEnd);
	++animIndex;
}

Rect Projectile::GetBounds() const
{
	return Rect(posX, posY, static_cast<float>(projectileDef.width), static_cast<float>(projectileDef.height));
}
