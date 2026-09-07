#pragma once
#include <Actor\Actor.h>

namespace Pixel
{
	class AnimationClip;
}

class Platform : public Pixel::Actor
{
	TYPE_DECLARATIONS(Platform, Actor)

public:
	Platform(const Pixel::Vector2& position, int tileCountX);
	~Platform();

	virtual Pixel::Rect GetBounds() const override;

private:
	virtual void Draw(const Pixel::Vector2& camStart, const Pixel::Vector2& camEnd) override;
	
private:
	const Pixel::AnimationClip* idleAnimtaion = nullptr;
	int tileCountX = 0;
};

