#pragma once
#include <Actor/Projectile.h>
#include <unordered_map>

enum class ProjectileType
{
	RockBuster = 0,
	Astar,
	AirShooter
};

namespace Pixel
{
	class AnimationClip;
}

class ProjectileManager
{
public:
	ProjectileManager();

	void LoadProjectileDefinition(ProjectileType type, const Pixel::AnimationClip* animation);
	void RegisterProjectilePool(const std::shared_ptr<Projectile>& projectile);

	void SpwnProjectile(ProjectileType type, const Pixel::Vector2& position, bool isForward);

private:
	std::vector<Projectile*> pool;
	std::unordered_map<ProjectileType, ProjectileDefination> projectileDefinitions;
};

