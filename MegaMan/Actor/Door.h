#pragma once
#include <Actor/Actor.h>

namespace Pixel
{
	class AnimationClip;
}

class Door : public Pixel::Actor
{
	TYPE_DECLARATIONS(Door, Actor)

public:
	Door(const Pixel::Vector2& position, const int tileCountY, bool isBossRoomDoor = false);

	virtual void OnCollision(const Actor* other) override;

private:
	virtual void Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd) override;

	void PlayCloseAnimation(bool isBackward);

private:
	bool isBossDoor = false;

	int tileCountY = 0;

	bool hasOpend = false;

	int idleIndex = 0;
	const Pixel::AnimationClip* idleAnimtaion = nullptr;
};

