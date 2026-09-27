#pragma once
#include <Actor/Actor.h>

namespace Pixel
{
	class AnimationClip;
}

class HornPlatform : public Pixel::Actor
{
	TYPE_DECLARATIONS(HornPlatform, Actor)

public:
	HornPlatform(const Pixel::Vector2& position = Pixel::Vector2::Zero);

private:
	virtual void Draw(const Pixel::Vector2& camStart, const Pixel::Vector2& camEnd) override;

private:
	const Pixel::AnimationClip* idleAnimtaion = nullptr;
};

