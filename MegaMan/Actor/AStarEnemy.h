#pragma once
#include <Actor/Enemy/Enemy.h>

class AStarEnemy : public Enemy
{
	TYPE_DECLARATIONS(AStarEnemy, Enemy)

public:
	AStarEnemy(const Pixel::Vector2& position = Pixel::Vector2::Zero);

	virtual Pixel::Rect GetBounds() const override;

	virtual void OnDamaged(const int damage) override;

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd) override;

	void ShootAStar();

private:
	const float shootInterval = 1.0f;
	float shootTimer = 0;
};