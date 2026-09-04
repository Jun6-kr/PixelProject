#pragma once
#include <Actor/Actor.h>
#include <Physics/Collision.h>
#include <vector>

namespace Pixel
{
	class Sprite;
}

class Platform;

enum class State
{
	Idle = 0,
	Run,
	Attack,
	Jump,
};

class Player : public Pixel::Actor
{

	TYPE_DECLARATIONS(Player, Actor)

public:
	Player(const Pixel::Vector2& position);
	~Player();

	inline void UpdateState(State newState) { actorState = newState; }

	virtual Pixel::Rect GetBounds() const override;

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd) override;

	void ProcessInput(float direction, float deltaTime);

	bool CanMove(float toX, float toY);
	void ResolveCollision(Pixel::CollisionResult result);

private:
	State actorState = State::Idle;

	int idleIndex = 0;
	std::vector<Pixel::Sprite*> idleSprite;
	int runningIndex = 0;
	std::vector<Pixel::Sprite*> runningSprite;

	bool enableInput = true;
	std::weak_ptr<Actor> standingPlatform;

	float posX = 0.f;
	float posY = 0.f;

	const float moveSpeed = 45.f;

	bool isJumping = false;
	const float jumpSpeed = 50.0f;
	float velocityY = 0.f;
	float gravity = 50.0f;
};

