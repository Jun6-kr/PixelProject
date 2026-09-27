#pragma once
#include <Actor/Actor.h>
#include <functional>

using HudCallback = std::function<void(const int)>;

namespace Pixel
{
	class Sprite;
	class AnimationClip;
}

class Platform;

enum class MovementState
{
	Idle = 0,
	Move,
	Attack,
	Jump,
};

enum class ActionState
{
	None,
	Attack,
	Damaged
};

class Player : public Pixel::Actor
{

	TYPE_DECLARATIONS(Player, Actor)

public:
	Player();
	~Player();

	inline void SetEnableInput(bool enableInput) { this->enableInput = enableInput; }

	inline void UpdateMovementState(MovementState newState)
	{ 
		if (movementState == newState)
		{
			return;
		}

		movementState = newState;
	}
	void SetAnimationClip(const int stateIndex, const Pixel::AnimationClip* clip);

	virtual void SetPosition(const Pixel::Vector2& newPosition);
	virtual Pixel::Rect GetBounds() const override
	{
		return Pixel::Rect(posX, posY, static_cast<float>(width), static_cast<float>(height));
	};

	void UpdateFloatPosition(float deltaTime);

	virtual void OnDamaged(const int damage) override;

	void BindDamageFunction(HudCallback callback)
	{ 
		OnHpChanged = std::move(callback);
		OnHpChanged(hp);
	}

	void Reset();

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd) override;

	bool CanMove(float toX, float toY);

	void UpdateActionState(float deltaTime);

	void ShootRockBuster();

private:
	MovementState movementState = MovementState::Idle;
	ActionState actionState = ActionState::None;

	HudCallback OnHpChanged;

	int idleIndex = 0;
	const Pixel::AnimationClip* idleAnimtaion = nullptr;

	int moveIndex = 0;
	const Pixel::AnimationClip* moveAnimation = nullptr;

	const Pixel::AnimationClip* jumpAnimation = nullptr;

	const Pixel::AnimationClip* jumpAttackAnimation = nullptr;

	const Pixel::AnimationClip* attackAnimation = nullptr;

	int deathIndex = 0;
	const Pixel::AnimationClip* deathAnimation = nullptr;

	bool enableInput = true;
	std::weak_ptr<Actor> standingPlatform;

	bool isForward = true;

	float posX = 0.f;
	float posY = 0.f;

	const float moveSpeed = 60.f;

	const float jumpSpeed = 50.0f;
	float velocityY = 0.f;
	float gravity = 60.0f;

	// Attack Timer
	const float attackDuration = 1.0f;
	float attackTimer = 0.0f;

	// Damaged Timer
	bool bIsDamaged = true;
	const float damageDuration = 2.0f;
	float damagedTimer = 0.0f;
	const float damagedInterval = 0.1f;
};

