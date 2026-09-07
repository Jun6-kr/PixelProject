#pragma once
#include "Enemy.h"

enum class AirmanPhase
{
	Phase1 = 0,
	Phase2,
	Phase3
};

enum class AirmanState
{
	Idle,
	Attack,
	Jump,
	Dead
};

class Airman : public Enemy
{
	TYPE_DECLARATIONS(Airman, Enemy)

public:
	Airman(const Pixel::Vector2& position = Pixel::Vector2::Zero);

	virtual void OnDamaged(const int damage);

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd) override;

private:
	AirmanState state = AirmanState::Idle;

	int attackIndex = 0;
	const Pixel::AnimationClip* attackAnimtaion = nullptr;
	const Pixel::AnimationClip* jumpAnimtaion = nullptr;

};

