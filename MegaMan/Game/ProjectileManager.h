#pragma once
#include <Actor/Projectile.h>
#include <unordered_map>

namespace Pixel
{
	class AnimationClip;
}

class GameLevel;

class ProjectileManager
{
public:
	ProjectileManager();

	void LoadProjectileDefinition(EProjectileType type, const Pixel::AnimationClip* animation);
	void RegisterProjectilePool(EProjectileType type, const std::shared_ptr<Projectile>& projectile);

	Projectile* SpwnProjectile(EProjectileType type, const Pixel::Vector2& position, const Pixel::Vector2& direction, GameLevel* level = nullptr);

private:
	std::vector<Projectile*> pool;
	std::unordered_map<EProjectileType, std::vector<Projectile*>> typePool;
	std::unordered_map<EProjectileType, ProjectileDefination> projectileDefinitions;

	const int maxAstarCount = 10;
};

