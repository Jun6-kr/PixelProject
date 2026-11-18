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
	Pixel::Vector2(160, -102),
	Pixel::Vector2(100, -95), 
	Pixel::Vector2(110, -13), 
	Pixel::Vector2(70, -48), 
	Pixel::Vector2(20, -58), 
	Pixel::Vector2(40, -74),
};

using HudCallback = std::function<void(const int)>;

class Airman : public Enemy
{
	TYPE_DECLARATIONS(Airman, Enemy)

public:
	Airman(const Pixel::Vector2& position = Pixel::Vector2::Zero);

	virtual void OnDamaged(const int damage);

	void BindDamageFunction(HudCallback callback)
	{
		OnHpChanged = std::move(callback);
		OnHpChanged(hp);
	}

	void StartPhase1();

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd) override;

	void SpawnWind();
	void FireAirShooter();
	void StartJump();

	void UpdateProjectileIsLocatedPosition(int projectileIndex);
	void UpdateProjectileDestroyed();
	void PlayNextPhase();

private:
	// Animation
	AirmanPhase currentPhase = AirmanPhase::Phase0;
	AnimationState currentState = AnimationState::Idle;
	bool isForward = false;

	int windIndex = 0;
	const Pixel::AnimationClip* windAnimtaion = nullptr;
	const Pixel::AnimationClip* airShooterAnimtaion = nullptr;
	//const Pixel::AnimationClip* jumpAnimtaion = nullptr;

	HudCallback OnHpChanged;

	// Projectile
	std::vector<AirShooter*> wind;
	std::vector<bool> bIsAirShooterDeployPositionArr;
	bool isShooterFired = false;
	bool isWindDestroyed = false;
	
	float phaseTimer = 0.0f;
	const float phauseTime = 1.5f;

	// Phase
	int airShootingCount = 0;
	const int maxAirShootingCount = 3;

	// Jump
	bool isJumping = false;

	Pixel::Vector2 jumpStart;
	Pixel::Vector2 jumpTarget[2];
	int jumpOffset[2] = { 50, 117 };
	int jumpHeight[2] = { 40, 110 };

	int jumpIndex = 0;
	
	float jumpTime = 0.0f;
	const float jumpDuration = 1.5f;
};

