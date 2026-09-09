#pragma once
#include <Actor\Actor.h>
#include <string>

namespace Pixel
{
	class AnimationClip;
}

class Platform : public Pixel::Actor
{
	TYPE_DECLARATIONS(Platform, Actor)

public:
	Platform(const std::string& id, const Pixel::Vector2& position, int tileCountX, int tileCountY);
	~Platform();

private:
	virtual void Draw(const Pixel::Vector2& camStart, const Pixel::Vector2& camEnd) override;
	
private:
	const Pixel::AnimationClip* idleAnimtaion = nullptr;
	int tileCountX = 0;
	int tileCountY = 0;
};

