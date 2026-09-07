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
	blockMask = 0b00001;
	damageMask = 0b10100;
	HP = 28;
}

Player::Player(const Vector2& position)
	: Actor(position), posX(static_cast<float>(position.x)), posY(static_cast<float>(position.y))
{
	width = 20;
	height = 20;

	actorType = ActorType::Player;
	blockMask = 0b00001;
	damageMask = 0b10100;
	HP = 28;
}

Player::~Player()
{
}

void Player::SetAnimationClip(State state, const Pixel::AnimationClip* clip)
{
	switch (state)
	{
	case State::Idle:
		idleAnimtaion = clip;
		break;
	case State::Move:
		moveAnimation = clip;
		break;
	case State::Jump:
		jumpAnimation = clip;
		break;
	case State::Attack:
		attackAnimation = clip;
		break;
	case State::JumpAttack:
		jumpAttackAnimation = clip;
	}
}

void Player::UpdateFloatPosition(const Pixel::Vector2& position)
{
	SetPosition(position);
	posX = static_cast<float>(position.x);
	posY = static_cast<float>(position.y);
}

void Player::BeginPlay()
{
	super::BeginPlay();
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
	}

	float moveX = 0.0f;

	if (Input::Get().GetKey(VK_LEFT))
	{
		isForward = false;

		if (currentState != State::Jump && currentState != State::JumpAttack && currentState != State::Attack)
		{
			UpdateState(State::Move);
		}
		moveX -= moveSpeed * deltaTime;
	}
	else if (Input::Get().GetKeyUp(VK_LEFT))
	{
		if (currentState != State::Jump && currentState != State::JumpAttack)
		{
			UpdateState(prevState);
		}
	}

	if (Input::Get().GetKey(VK_RIGHT))
	{
		isForward = true;

		if (currentState != State::Jump && currentState != State::JumpAttack)
		{
			UpdateState(State::Move);
		}
		moveX += moveSpeed * deltaTime;
	}
	else if (Input::Get().GetKeyUp(VK_RIGHT))
	{
		if (currentState != State::Jump && currentState != State::JumpAttack)
		{
			UpdateState(prevState);
		}
	}

	// 점프
	if (Input::Get().GetKeyDown(VK_UP) && currentState != State::Jump && currentState != State::JumpAttack)
	{
		UpdateState(State::Jump);
		velocityY -= jumpSpeed;
		standingPlatform.reset();
	}

	if (Input::Get().GetKeyDown(VK_SPACE))
	{
		if (currentState == State::Jump || currentState == State::JumpAttack)
		{
			UpdateState(State::JumpAttack);
		}
		else
		{
			UpdateState(State::Attack);
		}

		if (shootElapsed > shootInterval)
		{
			shootElapsed = 0;
			ShootRockBuster();
		}
		else
		{
			shootElapsed += deltaTime;
		}
	}
	else if (Input::Get().GetKeyUp(VK_SPACE))
	{
		UpdateState(prevState);
	}

	velocityY += gravity * deltaTime;
	float moveY = velocityY * deltaTime;

	if (CanMove(moveX, 0))
	{
		posX += moveX;
	}

	if (CanMove(0, moveY))
	{
		posY += moveY;
	}
	else
	{
		velocityY = 0;
		if (currentState == State::Jump || currentState == State::JumpAttack)
		{
			UpdateState(State::Idle);
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

	switch (currentState)
	{
	case State::Idle:
		if (idleIndex >= idleAnimtaion->GetSize())
		{
			idleIndex = 0;
		}
		idleAnimtaion->PlayAnimation(idleIndex, pos, screenEnd);
		++idleIndex;
		break;
	case State::Move:
		if (moveIndex >= moveAnimation->GetSize())
		{
			moveIndex = 0;
		}
		moveAnimation->PlayAnimation(moveIndex, pos, screenEnd);
		++moveIndex;
		break;
	case State::Jump:
		jumpAnimation->PlayAnimation(0, pos, screenEnd);
		break;
	case State::Attack:
		attackAnimation->PlayAnimation(attackIndex, pos, screenEnd);
		break;
	case State::JumpAttack:
		jumpAttackAnimation->PlayAnimation(0, pos, screenEnd);
		break;
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
	rect.left += toX;
	rect.height += toY;

	// Damage 계산은 Enemy / Bullet 쪽에서
	CollisionResult result = level->CheckCollision(this, rect);
	return !result.isBlocked;
}

void Player::ShootRockBuster()
{
	if (std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner()))
	{
		// 20, 11에서만 발사
		level->ShootProjectile(ProjectileType::RockBuster, position + Vector2(20, 9), Vector2(1, 0));
	}
}
