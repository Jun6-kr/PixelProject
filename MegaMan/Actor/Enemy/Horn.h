#pragma once
#include <Actor/Enemy/Enemy.h>

class Horn : public Enemy
{
	TYPE_DECLARATIONS(Horn, Enemy)

public:
	Horn(const Pixel::Vector2& position = Pixel::Vector2::Zero);

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw(const Pixel::Vector2& camStart, const Pixel::Vector2& camEnd) override;

	void CalculateHeight();

private:
	const Pixel::Vector2 basePosition;

	float movingTimer = 0.0f;
	float movingInterval = 0.5f;

	int movingDirection = 1;
	int size = 0;

	const int damage = 2;

	int animIndex = 0;
	const Pixel::AnimationClip* idleAnimtaion = nullptr;
};

