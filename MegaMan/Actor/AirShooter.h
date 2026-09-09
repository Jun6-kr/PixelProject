#pragma once
#include "Projectile.h"
#include <Math/AstarAlgorithm.h>

enum class AirShooterState
{
	Deploy,     // 생성 위치 -> 지정된 배치 위치
	Standby,    // 배치 완료 후 정지
	Fire        // 플레이어 위치를 향해 이동
};

class AirShooter : public Projectile
{
	TYPE_DECLARATIONS(AirShooter, Projectile)

public:
	AirShooter();

	void SetDeployTarget(const Pixel::Vector2& target);
	void Fire();

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd) override;

	void MoveToDeployPosition(float deltaTime);

private:
	Pixel::AstarAlgorithm aStar;
	Pixel::Vector2 deployTarget;

	int pathIndex = 0;
	std::vector<Pixel::Vector2> deployPath;

	float moveTimer = 0.0f;

	AirShooterState state = AirShooterState::Deploy;
	float fireSpeed = 40.0f;
};

