#pragma once
#include <Actor/Actor.h>

namespace Pixel
{
	class AnimationClip;
}

class AStarEnemy : public Pixel::Actor
{
	TYPE_DECLARATIONS(AStarEnemy, Actor)

public:
	AStarEnemy();

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd) override;

private:
	const Pixel::AnimationClip* idleAnimtaion = nullptr;
};

