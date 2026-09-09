#pragma once
#include "Enemy.h"
#include <vector>

enum class AirmanPhase
{
	Phase0 = 0,
	Phase1,
	Phase2,
	Phase3
};

enum class AnimationState
{
	Idle,
	Attack,
	Wind,
	Jump,
	Dead
};

class AirShooter;

const Pixel::Vector2 deployOffsets[6] =
{
	Pixel::Vector2(18,  -8),   // 보스 바로 앞, 낮게
	Pixel::Vector2(10, -34),   // 가까운 위쪽
	Pixel::Vector2(42, -22),   // 중간 전방
	Pixel::Vector2(58, -48),   // 중간 위쪽
	Pixel::Vector2(86, -14),   // 먼 전방
	Pixel::Vector2(104, -42)   // 먼 위쪽
};

class Airman : public Enemy
{
	TYPE_DECLARATIONS(Airman, Enemy)

public:
	Airman(const Pixel::Vector2& position = Pixel::Vector2::Zero);

	virtual void OnDamaged(const int damage);

	

	void StartPhase1();

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd) override;

	void RunPhase1(float deltaTime);
	void RunPhase2(float deltaTime);
	void RunPhase3(float deltaTime);

	void SpawnAirShooters();

	bool IsAnimationFinished();

private:
	AirmanPhase currentPhase = AirmanPhase::Phase0;
	AnimationState currentState = AnimationState::Idle;

	bool isForward = false;

	const float phase1Time = 3.0f;
	float phaseTimer = 0.0f;

	bool attackExecuted = false;

	std::vector<AirShooter*> airShooter;

	int windIndex = 0;
	const Pixel::AnimationClip* windAnimtaion = nullptr;
	const Pixel::AnimationClip* airShooterAnimtaion = nullptr;
	//const Pixel::AnimationClip* jumpAnimtaion = nullptr;

};

