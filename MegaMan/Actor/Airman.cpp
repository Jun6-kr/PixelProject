#include "Airman.h"
#include <Game/GameManager.h>
#include <Level/GameLevel.h>
#include <Actor/AirShooter.h>

#include <System/DebugSystem.h>

using namespace Pixel;

Airman::Airman(const Vector2& position) : Enemy(position)
{
	actorType = ActorType::Enemy;

	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	idleAnimtaion = game.GetClip("Airman_Idle");
	windAnimtaion = game.GetClip("Airman_Wind");
	airShooterAnimtaion = game.GetClip("Airman_Airshooter");
	//jumpAnimtaion = game.GetClip("Airman_Jump");

	blockMask = 0b00000;
	damageMask = 0b00001;

	hp = 28;
}

void Airman::OnDamaged(const int damage)
{
	if (hp <= damage)
	{
		GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
		game.FinishEffect(position);
		SetActive(false);
	}

	hp -= damage;
}

void Airman::StartPhase1()
{
	currentPhase = AirmanPhase::Phase1;
	phaseTimer = 0.0f;
}

void Airman::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	switch (currentPhase)
	{
	case AirmanPhase::Phase1:
		RunPhase1(deltaTime);
		break;
	case AirmanPhase::Phase2:
		RunPhase2(deltaTime);
		break;
	case AirmanPhase::Phase3:
		RunPhase3(deltaTime);
		break;
	}

}

void Airman::Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd)
{
	Vector2 pos = position - screenOffset;

	switch (currentState)
	{
	case AnimationState ::Idle:
		idleAnimtaion->PlayAnimation(0, pos, screenEnd, !isForward);
		break;
	case AnimationState ::Attack:
		airShooterAnimtaion->PlayAnimation(0, pos, screenEnd, !isForward);
		break;
	case AnimationState ::Wind:
		if (windIndex >= windAnimtaion->GetSize())
		{
			windIndex = 0;
		}
		windAnimtaion->PlayAnimation(windIndex, pos, screenEnd, !isForward);
		++windIndex;
		break;
	//case AirmanState ::Jump:
	//	jumpAnimtaion->PlayAnimation(0, pos, screenEnd);
		//break;

	}
}

void Airman::RunPhase1(float deltaTime)
{
	phaseTimer += deltaTime;

	if (!attackExecuted)
	{
		if (phaseTimer >= 1.0f)
		{
			attackExecuted = true;
			currentState = AnimationState::Attack;
			SpawnAirShooters();
		} 

		return;
	}

	// 공격 모션 종료
	if (IsAnimationFinished())
	{
		currentPhase = AirmanPhase::Phase2;
		phaseTimer = 0.0f;
		attackExecuted = false;

		currentState = AnimationState::Idle;
	}
}

void Airman::RunPhase2(float deltaTime)
{
	for (AirShooter*& shooter : airShooter)
	{
		if (shooter && shooter->IsActive())
		{
			shooter->Fire();
		}
	}
}

void Airman::RunPhase3(float deltaTime)
{
	currentPhase = AirmanPhase::Phase3;
}

void Airman::SpawnAirShooters()
{
	if (std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner()))
	{
		Vector2 spawnPosition =	position + Vector2(isForward ? 3 : 29, 11);

		for (int i = 0; i < 6; ++i)
		{
			Projectile* projectile = level->ShootProjectile(ProjectileType::AirShooter
				, spawnPosition, Vector2(isForward ? 1 : -1, 0));

			if (!projectile)
			{
				continue;
			}

			AirShooter* shooter = dynamic_cast<AirShooter*>(projectile);

			if (!shooter)
			{
				continue;
			}

			int dirX = isForward ? 1 : -1;
			Vector2 target = spawnPosition + Vector2(deployOffsets[i].x * dirX,	deployOffsets[i].y);
			JxDebug::DebugSystem::Get().AddBox(target.x, target.y, width, height, 0x0D);

			shooter->SetDeployTarget(target);
			airShooter.emplace_back(shooter);
		}
	}
}

bool Airman::IsAnimationFinished()
{
	return phaseTimer > phase1Time;
}
