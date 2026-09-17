#pragma once
#include "Projectile.h"
#include <Math/AstarAlgorithm.h>
#include <vector>

class AStarBullet : public Projectile
{
	TYPE_DECLARATIONS(AStarBullet, Projectile)

public:
	AStarBullet();
	virtual void OnSpawn(GameLevel* level) override;

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd) override;
	virtual void OnCollision(const Actor* other);

private:
	Pixel::AstarAlgorithm aStar;
	int astarIndex = 0;
	std::vector<Pixel::Vector2> route;

	float moveTimer = 0.0f;
};