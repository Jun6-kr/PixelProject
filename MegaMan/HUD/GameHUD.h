#pragma once

namespace Pixel
{
	class AnimationClip;
}

class GameHUD
{
public:
	GameHUD();

	void Tick(float deltaTime);

	void OnPlayerDamaged(int hp, const int maxHp);
	void OnAirmanDamaged(int hp, const int maxHp);

	void OnAirmanPhaseBegan();

private:
	const AnimationClip* HpBarSprite = nullptr;
	const AnimationClip* BossHp = nullptr;
	const AnimationClip* PlayerHp = nullptr;
};