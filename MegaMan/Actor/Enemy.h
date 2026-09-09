#pragma once
#include <Actor/Actor.h>

namespace Pixel
{
	class AnimationClip;
}

class Enemy : public Pixel::Actor
{
	TYPE_DECLARATIONS(Enemy, Actor)

public:
	Enemy(const Pixel::Vector2& position = Pixel::Vector2::Zero);

protected:
	const Pixel::AnimationClip* idleAnimtaion = nullptr;
};

