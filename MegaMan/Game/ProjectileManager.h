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

	void LoadProjectileDefinition(ProjectileType type, const Pixel::AnimationClip* animation);
	void RegisterProjectilePool(ProjectileType type, const std::shared_ptr<Projectile>& projectile);

	Projectile* SpwnProjectile(ProjectileType type, const Pixel::Vector2& position, const Pixel::Vector2& direction, GameLevel* level = nullptr);

private:
	std::vector<Projectile*> pool;
	std::unordered_map<ProjectileType, std::vector<Projectile*>> typePool;
	std::unordered_map<ProjectileType, ProjectileDefination> projectileDefinitions;
};

