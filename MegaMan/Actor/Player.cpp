#include "Player.h"
#include <Input/Input.h>
#include <Render/Sprite.h>
#include <Level/GameLevel.h>
#include <Debug/DebugAdapter.h>
#include <System/DebugSystem.h>

using namespace Pixel;

Player::Player(const Vector2& position) : Actor(position), posX(static_cast<float>(position.x)), posY(static_cast<float>(position.y))
{
	actorType = ActorType::Player;

	idleSprite.emplace_back(new Sprite("../Assets/Player/Idle/Idle_1.txt", actorType));


	runningSprite.emplace_back(new Sprite("../Assets/Player/Forward/Forward_1.txt", actorType));
	runningSprite.emplace_back(new Sprite("../Assets/Player/Forward/Forward_2.txt", actorType));
	runningSprite.emplace_back(new Sprite("../Assets/Player/Forward/Forward_3.txt", actorType));
}

Player::~Player()
{
	for (Sprite* spr : idleSprite)
	{
		delete spr;
		spr = nullptr;
	}
	for (Sprite* spr : runningSprite)
	{
		delete spr;
		spr = nullptr;
	}
}

Rect Player::GetBounds() const
{
	//Rect rect = { };
	//switch (state)
	//{
	//case State::Idle:
	//	break;
	//case State::Run:
	//	break;
	//case State::Jump:
	//	break;
	//case State::Attack:
	//	break;
	//}

	// TODO setting으로 처리?
	int width = idleSprite[idleIndex]->GetSpriteWidth() - 1;
	int height= idleSprite[idleIndex]->GetSpriteHeight() - 1;


	return Rect(posX, posY, static_cast<float>(width), static_cast<float>(height));
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
		UpdateState(State::Run);
		moveX -= moveSpeed * deltaTime;
	}
	else if (Input::Get().GetKeyUp(VK_LEFT))
	{
		UpdateState(State::Idle);
	}

	if (Input::Get().GetKey(VK_RIGHT))
	{
		UpdateState(State::Run);
		moveX += moveSpeed * deltaTime;
	}
	else if (Input::Get().GetKeyUp(VK_RIGHT))
	{
		UpdateState(State::Idle);
	}

	// 점프
	if (Input::Get().GetKeyDown(VK_SPACE) && !isJumping)
	{
		isJumping = true;
		velocityY -= jumpSpeed;
		standingPlatform.reset();
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
		isJumping = false;
	}

	Vector2 newPosition = GetPosition();
	newPosition.x = static_cast<int>(posX);
	newPosition.y = static_cast<int>(posY);

	SetPosition(newPosition);
	if (std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner()))
	{
		level->UpdateCamera(posX);
	}

	//int width = idleSprite[idleIndex]->GetSpriteWidth();
	//int height = idleSprite[idleIndex]->GetSpriteHeight();
	//JxDebug::DebugBox box = DebugAdapter::Get().MakeDebugBox(position, width, height);
	//JxDebug::DebugSystem::Get().AddBox(box);
}

void Player::Draw(const Vector2& screenOffset, const Vector2& screenEnd)
{
	Vector2 pos = position - screenOffset;

	switch (actorState)
	{
	case State::Idle:
		if (idleSprite.empty())
		{
			return;
		}

		if (idleIndex > idleSprite.size())
		{
			idleIndex = 0;
		}

		idleSprite[idleIndex]->Draw(pos, screenEnd);
		idleIndex++;
		idleIndex %= idleSprite.size();
		break;
	case State::Run:
		if (runningSprite.empty())
		{
			return;
		}

		if (runningIndex > runningSprite.size())
		{
			runningIndex = 0;
		}

		runningSprite[runningIndex]->Draw(pos, screenEnd);
		runningIndex++;
		runningIndex %= runningSprite.size();
		break;
	case State::Jump:
		break;
	case State::Attack:
		break;
	}
}

void Player::ProcessInput(float direction, float deltaTime)
{
	if (Input::Get().GetKey(VK_LEFT))
	{
		UpdateState(State::Run);

		float amount = direction * moveSpeed * deltaTime;
		float newPosX = posX + amount;

		Vector2 newPosition = GetPosition();
		newPosition.x -= 2;

		if (newPosition.x < 0)
		{
			newPosition.x = 0;
		}

		SetPosition(newPosition);
	}
	else if (Input::Get().GetKeyUp(VK_LEFT))
	{
		UpdateState(State::Idle);
	}

	if (Input::Get().GetKey(VK_RIGHT))
	{
		UpdateState(State::Run);

		Vector2 newPosition = GetPosition();
		newPosition.x += 2;

		SetPosition(newPosition);
	}
	else if (Input::Get().GetKeyUp(VK_RIGHT))
	{
		UpdateState(State::Idle);
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

	CollisionResult result = level->CheckCollision(rect);
	if (result.hasCollision)
	{
		ResolveCollision(result);
		return !result.isBlocked;
	}

	return true;
}

void Player::ResolveCollision(CollisionResult result)
{
	switch(result.actor.lock()->GetActorType())
	{
	case ActorType::Bullet:
	case ActorType::Enemy:
		break;
	case ActorType::Terrain:
		standingPlatform = result.actor;
		break;
	}
}
