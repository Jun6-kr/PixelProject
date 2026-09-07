#pragma once
#include <Actor/Actor.h>

namespace Pixel
{
	class Sprite;
	class AnimationClip;
}

class Platform;

enum class State
{
	Idle = 0,
	Move = 0b0001,
	Attack = 0b0010,
	Jump = 0b0100,
	JumpAttack = Attack | Jump,
};

class Player : public Pixel::Actor
{

	TYPE_DECLARATIONS(Player, Actor)

public:
	Player();
	Player(const Pixel::Vector2& position);
	~Player();

	inline void UpdateState(State newState)
	{ 
		if (currentState == newState)
		{
			return;
		}

		prevState = currentState;
		currentState = newState;
	}
	void SetAnimationClip(State state, const Pixel::AnimationClip* clip);

	virtual Pixel::Rect GetBounds() const override
	{
		return Pixel::Rect(posX, posY, static_cast<float>(width), static_cast<float>(height));
	};

	void UpdateFloatPosition(const Pixel::Vector2& position);

private:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd) override;

	bool CanMove(float toX, float toY);

	void ShootRockBuster();

private:
	State currentState = State::Idle;
	State prevState = State::Idle;

	int idleIndex = 0;
	const Pixel::AnimationClip* idleAnimtaion = nullptr;

	int moveIndex = 0;
	const Pixel::AnimationClip* moveAnimation = nullptr;

	const Pixel::AnimationClip* jumpAnimation = nullptr;

	const Pixel::AnimationClip* jumpAttackAnimation = nullptr;

	int attackIndex = 0;
	const Pixel::AnimationClip* attackAnimation = nullptr;

	bool enableInput = true;
	std::weak_ptr<Actor> standingPlatform;

	bool isForward = true;

	float posX = 0.f;
	float posY = 0.f;

	const float moveSpeed = 45.f;

	bool isJumping = false;
	const float jumpSpeed = 50.0f;
	float velocityY = 0.f;
	float gravity = 50.0f;

	const float shootInterval = 0.01f;
	float shootElapsed = 10.0f;
};

