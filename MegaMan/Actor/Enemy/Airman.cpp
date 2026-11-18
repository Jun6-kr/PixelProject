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


	width = idleAnimtaion->GetWidth();
	height = idleAnimtaion->GetHeight();

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
	OnHpChanged(hp);
}

void Airman::StartPhase1()
{
	JxDebug::DebugSystem::Get().Print("Phase", 1);

	currentPhase = AirmanPhase::Phase1;
	currentState = AnimationState::Attack;
	SpawnWind();
	phaseTimer = phauseTime;
}

void Airman::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (phaseTimer > 0)
	{
		JxDebug::DebugSystem::Get().Print("phaseTimer", phaseTimer);
		phaseTimer -= deltaTime;
	}
	else
	{
		switch (currentPhase)
		{
		case AirmanPhase::Phase1:
			if (currentState != AnimationState::Idle)
			{
				currentState = AnimationState::Idle;
			}

			if (isShooterFired)
			{
				isShooterFired = false;
				phaseTimer = 6.0f;
				currentPhase = AirmanPhase::Phase2;
				currentState = AnimationState::Wind;
				FireAirShooter();
			}
			break;
		case AirmanPhase::Phase2:
			JxDebug::DebugSystem::Get().Print("Phase", 2);

			currentState = AnimationState::Idle;

			++airShootingCount;
			JxDebug::DebugSystem::Get().Print("All Destroyed", airShootingCount);
			wind.clear();
			bIsAirShooterDeployPositionArr.clear();

			if (airShootingCount == maxAirShootingCount)
			{
				PlayNextPhase();
			}
			else
			{
				phaseTimer = /*phauseTime*/2.0f;

				StartPhase1();
			}

			break;
		case AirmanPhase::Phase3:
			if (isJumping)
			{
				jumpTime += deltaTime;
				float t = jumpTime / jumpDuration;

				if (t > 1.0f)
				{
					t = 1.0f;
				}

				float x = jumpStart.x + (jumpTarget[jumpIndex].x - jumpStart.x) * t;
				float y = jumpStart.y - jumpHeight[jumpIndex] * 4.0f * t * (1.0f - t);

				position.x = static_cast<int>(x);
				position.y = static_cast<int>(y);

				if (t >= 1.0f)
				{
					position = jumpTarget[jumpIndex];
					++jumpIndex;
					jumpTime = 0.0f;
					
					if (jumpIndex < 2)
					{
						StartJump();
					}
					else
					{
						isJumping = false;
						isForward = !isForward;
						jumpIndex = 0;
						phaseTimer = phauseTime;
						StartPhase1();
					}
				}
			}

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

void Airman::SpawnWind()
{
	if (std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner()))
	{
		Vector2 spawnPosition =	position + Vector2(isForward ? 20 : -20, 11);

		for (int i = 0; i < 6; ++i)
		{
			Projectile* projectile = level->ShootProjectile(EProjectileType::AirShooter
				, spawnPosition, Vector2(isForward ? 1 : -1, 0));

			if (!projectile)
			{
				JxDebug::DebugSystem::Get().Print("there is bo shoot projectile in ", 0);
				continue;
			}

			AirShooter* shooter = dynamic_cast<AirShooter*>(projectile);

			if (!shooter)
			{
				continue;
			}

			JxDebug::DebugSystem::Get().Print("SpawnWind", i);

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
	else
	{
		JxDebug::DebugSystem::Get().Print("Fail to spawn wind - no game level", 0);
	}
}

void Airman::FireAirShooter()
{
	for (AirShooter*& shooter : wind)
	{
		if (shooter && shooter->IsActive())
		{
			shooter->Fire();
		}
	}
}

void Airman::StartJump()
{
	int dirX = isForward ? 1 : -1;
	jumpStart = position;
	jumpTarget[jumpIndex] = position + Vector2(jumpOffset[jumpIndex] * dirX, 0);

	isJumping = true;
}

void Airman::UpdateProjectileIsLocatedPosition(int projectileIndex)
{
	if (bIsAirShooterDeployPositionArr.empty())
	{
		return;
	}

	if (projectileIndex >= 0 && projectileIndex < bIsAirShooterDeployPositionArr.size())
	{
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
		phaseTimer = 2.0f;
		isShooterFired = true;
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
		StartJump();
		break;
	case AirmanPhase::Phase3:
		break;
	}
}
