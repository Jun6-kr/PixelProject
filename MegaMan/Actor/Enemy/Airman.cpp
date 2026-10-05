#include "Airman.h"
#include <Game/GameManager.h>
#include <Level/GameLevel.h>
#include <Actor/Enemy/AirShooter.h>

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
	currentState = AnimationState::Attack;
	SpawnAirShooters();
}

void Airman::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (phaseTimer > 0)
	{
		phaseTimer -= deltaTime;
	}
	else
	{
		switch (currentPhase)
		{
		case AirmanPhase::Phase1:
			if (isShooterFired)
			{
				isShooterFired = false;
				SpawnAirShooters();
			}
			break;
		case AirmanPhase::Phase2:
			JxDebug::DebugSystem::Get().Print("Phase2", 0);

			for (AirShooter*& shooter : wind)
			{
				if (shooter && shooter->IsActive())
				{
					shooter->Fire();
				}
			}


			if (airShootingCount == maxAirShootingCount)
			{
				PlayNextPhase();
			}
			else
			{
				++airShootingCount;
				//bIsAirShooterDeployPositionArr.clear();
				JxDebug::DebugSystem::Get().Print("Shoot again", 0);

				phaseTimer = /*phauseTime*/3.0f;
				currentPhase = AirmanPhase::Phase1;
				isShooterFired = true;
				//StartPhase1();
			}
			break;
		case AirmanPhase::Phase3:
			break;
		default:
			break;
		}
	}
}

void Airman::Draw(const Vector2& screenOffset, const Vector2& screenEnd)
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

			shooter->SetDeployTarget(target);
			shooter->SetProjectileIndex(i);
			shooter->deployCallback = [&](int index) {
				UpdateProjectileIsLocatedPosition(index);
			};

			bIsAirShooterDeployPositionArr.emplace_back(false);
			wind.emplace_back(shooter);
		}
	}
}

void Airman::UpdateProjectileIsLocatedPosition(int projectileIndex)
{
	if (bIsAirShooterDeployPositionArr.empty())
	{
		return;
	}

	if (projectileIndex >= 0 && projectileIndex < bIsAirShooterDeployPositionArr.size())
	{
		JxDebug::DebugSystem::Get().Print("set to true", projectileIndex);
		bIsAirShooterDeployPositionArr[projectileIndex] = true;
	}

	bool result = true;
	for (const bool& isOk : bIsAirShooterDeployPositionArr)
	{
		if (!isOk)
		{
			result = false;
			break;
		}
	}

	if (result)
	{
		JxDebug::DebugSystem::Get().Print("Time to Shoot", 0);
		PlayNextPhase();
	}
}

void Airman::PlayNextPhase()
{
	switch (currentPhase)
	{
	case AirmanPhase::Phase1:
		phaseTimer = phauseTime;
		currentPhase = AirmanPhase::Phase2;
		break;
	case AirmanPhase::Phase2:
		airShootingCount = 0;
		currentPhase = AirmanPhase::Phase3;
		break;
	case AirmanPhase::Phase3:
		//attackExecuted = false;
		//StartPhase1();
		break;
	}
}
