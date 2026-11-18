#include "ProjectileManager.h"
#include <System/DebugSystem.h>

using namespace Pixel;

ProjectileManager::ProjectileManager()
{
}

void ProjectileManager::LoadProjectileDefinition(EProjectileType type, const Pixel::AnimationClip* animation)
{
	ProjectileDefination def = {};
	def.type = type;

	switch (type)
	{
	case EProjectileType::RockBuster:
		def.damage = 2;
		def.lifeTime = -1;
		def.speed = 100;
		def.width = 7;
		def.height = 5;
		def.blockMask = 0b01100;
		def.damageMask = 0b01000;
		break;
	case EProjectileType::Astar:
		def.damage = 2;
		def.lifeTime = 10;
		def.speed = 70;
		def.width = 7;
		def.height = 5;
		def.blockMask = 0b01010;
		def.damageMask = 0b10000;
		break;
	case EProjectileType::AirShooter:
		def.damage = 4;
		def.lifeTime = 10;
		def.speed = 60;
		def.width = 7;
		def.height = 5;
		def.blockMask = 0b00100;
		def.damageMask = 0b10000;
		break;
	}

	def.animation = animation;
	projectileDefinitions[type] = def;
}

void ProjectileManager::RegisterProjectilePool(EProjectileType type, const std::shared_ptr<Projectile>& projectile)
{
	typePool[type].emplace_back(projectile.get());
}

Projectile* ProjectileManager::SpwnProjectile(EProjectileType type, const Pixel::Vector2& position, const Pixel::Vector2& direction, GameLevel* level)
{
	Projectile* projectile = nullptr;

	if (typePool[type].empty())
	{
		JxDebug::DebugSystem::Get().Print("pool is empty", 0);
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
	
	if (!projectile)
	{
		JxDebug::DebugSystem::Get().Print("pool is not empty, there is no inactive projectile", 0);
		return nullptr;
	}

	ProjectileDefination def = projectileDefinitions[type];
	projectile->SetProjectileInfo(def, position, direction, level);
	projectile->SetActive(true);

	return projectile;
}
