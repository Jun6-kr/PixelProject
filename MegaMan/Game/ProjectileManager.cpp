#include "ProjectileManager.h"

using namespace Pixel;

ProjectileManager::ProjectileManager()
{
}

void ProjectileManager::LoadProjectileDefinition(ProjectileType type, const Pixel::AnimationClip* animation)
{
	ProjectileDefination def = {};
	def.type = type;

	switch (type)
	{
	case ProjectileType::RockBuster:
		def.damage = 2;
		def.lifeTime = -1;
		def.speed = 100;
		def.width = 7;
		def.height = 5;
		def.blockMask = 0b01110;
		def.damageMask = 0b01000;
		break;
	case ProjectileType::Astar:
		def.damage = 2;
		def.lifeTime = 10;
		def.speed = 70;
		def.width = 7;
		def.height = 5;
		def.blockMask = 0b01011;
		def.damageMask = 0b10000;
		break;
	case ProjectileType::AirShooter:
		def.damage = 4;
		def.lifeTime = 10;
		def.speed = 80;
		def.width = 7;
		def.height = 5;
		def.blockMask = 0b00100;
		def.damageMask = 0b10000;
		break;
	}

	def.animation = animation;
	projectileDefinitions[type] = def;
}

void ProjectileManager::RegisterProjectilePool(ProjectileType type, const std::shared_ptr<Projectile>& projectile)
{
	typePool[type].emplace_back(projectile.get());
}

Projectile* ProjectileManager::SpwnProjectile(ProjectileType type, const Pixel::Vector2& position, const Pixel::Vector2& direction, GameLevel* level)
{
	Projectile* projectile = nullptr;

	if (typePool[type].empty())
	{
		return nullptr;
	}
	 
 	for (int ix = 0; ix < typePool[type].size(); ++ix)
	{
		if (!(typePool[type][ix]->IsActive()))
		{
			projectile = typePool[type][ix];
			break;
		}
	}

	ProjectileDefination def = projectileDefinitions[type];
	projectile->SetProjectileInfo(def, position, direction, level);
	projectile->SetActive(true);

	return projectile;
}
