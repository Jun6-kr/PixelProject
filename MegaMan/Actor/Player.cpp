#include "Player.h"
#include <Input/Input.h>
#include <Render/AnimationClip.h>
#include <Level/GameLevel.h>

using namespace Pixel;

Player::Player()
{
	width = 20;
	height = 20;

	actorType = ActorType::Player;
	blockMask = 0b00100;
	damageMask = 0b01010;
	hp = 28;
	maxHp = 28;
}

Player::~Player()
{
}

void Player::SetAnimationClip(const int stateIndex, const Pixel::AnimationClip* clip)
{
	switch (stateIndex)
	{
	case 0:
		idleAnimtaion = clip;
		break;
	case 1:
		moveAnimation = clip;
		break;
	case 2:
		attackAnimation = clip;
		break;
	case 3:
		jumpAnimation = clip;
		break;
	case 4:
		jumpAttackAnimation = clip;
	}
}

void Player::SetPosition(const Vector2& newPosition)
{
	posX = static_cast<float>(newPosition.x);
	posY = static_cast<float>(newPosition.y);
	super::SetPosition(newPosition);
}

void Player::UpdateFloatPosition(float deltaTime)
{
	posX += moveSpeed * deltaTime;

	Vector2 newPosition = GetPosition();
	newPosition.x = static_cast<int>(posX);

	if (posY < 85 || posY > 110)
	{
		posY = 100.f;
	}

	newPosition.y = static_cast<int>(posY);
	SetPosition(newPosition);
}

void Player::OnDamaged(const int damage)
{
	actionState = ActionState::Damaged;
	damagedTimer = damageInterval;

	if (damage >= hp)
	{

		return;
	}

	super::OnDamaged(damage);
	OnHpChanged(hp);
}

void Player::Reset()
{
	velocityY = 0;
	movementState = MovementState::Idle;
	actionState = ActionState::None;
	hp = 20;
	OnHpChanged(hp);
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (!enableInput)
	{
		return;
	}

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
	}

	float deltaX = 0.0f;

	if (Input::Get().GetKey(VK_LEFT))
	{
		isForward = false;
		deltaX -= moveSpeed * deltaTime;
	} 

	if (Input::Get().GetKey(VK_RIGHT))
	{
		isForward = true;
		deltaX += moveSpeed * deltaTime ;
	}

	// 점프
	if (Input::Get().GetKeyDown(VK_UP))
	{
		if (movementState != MovementState::Jump)
		{
			movementState = MovementState::Jump;
			velocityY -= jumpSpeed;
			standingPlatform.reset();
		}
	}

	if (movementState != MovementState::Jump)
	{
		if (deltaX != 0.f)
		{
			movementState = MovementState::Move;
		}
		else
		{
			movementState = MovementState::Idle;
		}
	}


	if (Input::Get().GetKeyDown(VK_SPACE))
	{
		if (actionState != ActionState::Damaged)
		{
			ShootRockBuster();
		}
	}

	UpdateActionState(deltaTime);

	velocityY += gravity * deltaTime;
	float deltaY = velocityY * deltaTime;

	if (CanMove(deltaX, 0))
	{
		posX += deltaX;
	}

	if (CanMove(0, deltaY))
	{
		posY += deltaY;
	}
	else
	{
		velocityY = 0;

		if (movementState == MovementState::Jump)
		{
			movementState = MovementState::Idle;
		}
	}

	Vector2 newPosition = GetPosition();
	newPosition.x = static_cast<int>(posX);
	newPosition.y = static_cast<int>(posY);
	SetPosition(newPosition);

	if (std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner()))
	{
		level->UpdateCamera(posX);
	}
}

void Player::Draw(const Vector2& screenOffset, const Vector2& screenEnd)
{
	Vector2 pos = position - screenOffset;

	if (actionState == ActionState::Attack)
	{
		if (movementState == MovementState::Jump)
		{
			jumpAttackAnimation->PlayAnimation(0, pos, screenEnd, isForward);
		}
		else
		{
			attackAnimation->PlayAnimation(windIndex, pos, screenEnd, isForward);
		}
	}
	else
	{
		switch (movementState)
		{
		case MovementState::Idle:
			if (idleIndex >= idleAnimtaion->GetSize())
			{
				idleIndex = 0;
			}
			idleAnimtaion->PlayAnimation(idleIndex, pos, screenEnd, isForward);
			++idleIndex;
			break;
		case MovementState::Move:
			if (moveIndex >= moveAnimation->GetSize())
			{
				moveIndex = 0;
			}
			moveAnimation->PlayAnimation(moveIndex, pos, screenEnd, isForward);
			++moveIndex;
			break;
		case MovementState::Jump:
			jumpAnimation->PlayAnimation(0, pos, screenEnd, isForward);
			break;
		}
	}
}

bool Player::CanMove(float toX, float toY)
{
	std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner());
	if (!level)
	{
		return false;
	}

	Rect rect = GetBounds();
	rect.x += toX;
	rect.y += toY;

	// Damage 계산은 Enemy / Bullet 쪽에서
	CollisionResult result = level->CheckCollision(this, rect);
	return !result.isBlocked;
}

void Player::UpdateActionState(float deltaTime)
{
	switch (actionState)
	{
	case ActionState::Attack:
		attackTimer -= deltaTime;

		if (attackTimer <= 0.0f)
		{
			actionState = ActionState::None;
		}
		break;
		
	case ActionState::Damaged:
		damagedTimer -= deltaTime;

		if (damagedTimer <= 0.0f)
		{
			actionState = ActionState::None;
		}
		break;
	}
}

void Player::ShootRockBuster()
{
	if (std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner()))
	{
		actionState = ActionState::Attack;
		attackTimer = attackInterval;

		// 20, 11에서만 발사
		level->ShootProjectile(ProjectileType::RockBuster, position + Vector2(isForward ? 20 : 0, 9), Vector2(isForward ? 1 : -1, 0));
	}
}
