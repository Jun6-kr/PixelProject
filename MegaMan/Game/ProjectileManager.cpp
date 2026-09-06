#include "ProjectileManager.h"

using namespace Pixel;

ProjectileManager::ProjectileManager()
{
}

void ProjectileManager::LoadProjectileDefinition(ProjectileType type, const Pixel::AnimationClip* animation)
{
	ProjectileDefination def = {};

	switch (type)
	{
	case ProjectileType::RockBuster:
		def.damage = 2;
		def.lifeTime = -1;
		def.speed = 100;
		def.width = 7;
		def.height = 5;
		def.blockMask = 0b00101;
		def.damageMask = 0b00100;
		break;
	case ProjectileType::Astar:
		def.damage = 2;
		def.lifeTime = 10;
		def.speed = 70;
		def.width = 7;
		def.height = 5;
		def.blockMask = 0b01011;
		def.damageMask = 0b00010;
		break;
	case ProjectileType::AirShooter:
		def.damage = 4;
		def.lifeTime = 10;
		def.speed = 80;
		def.width = 7;
		def.height = 5;
		def.blockMask = 0b10000;
		def.damageMask = 0b00010;
		break;
	}

	def.animation = animation;
	projectileDefinitions[type] = def;
}

void ProjectileManager::RegisterProjectilePool(const std::shared_ptr<Projectile>& projectile)
{
	pool.emplace_back(projectile.get());
}

void ProjectileManager::SpwnProjectile(ProjectileType type, const Pixel::Vector2& position, bool isForward)
{
	Projectile* projectile = nullptr;

	for (int ix = 0; ix < pool.size(); ++ix)
	{
		if (!(pool[ix]->IsActive()))
		{
			projectile = pool[ix];
			break;
		}
	}

	ProjectileDefination def = projectileDefinitions[type];
	projectile->SetProjectileInfo(def, position, isForward ? Vector2(1, 0) : Vector2(-1, 0));
	projectile->SetActive(true);
}
