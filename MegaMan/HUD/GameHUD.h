#pragma once
#include <Math/Vector2.h>
#include <vector>
#include <string>
#include <functional>

namespace Pixel
{
	class AnimationClip;
}

using OnFinished = std::function<void()>;

class GameHUD
{
public:
	GameHUD();

	void Tick(float deltaTime);

	void OnBossHpAnimationFinished(OnFinished onFinished) { this->onFinished = std::move(onFinished); }

	void OnPlayerHpChanged(const int hp);
	void OnAirmanHpChanged(const int hp);

	void OnAirmanPhaseBegan();

private:
	void DrawHPBar(const std::vector<std::string>& bar, const Pixel::Vector2& position, const int currentHp);

private:
	const Pixel::Vector2 playerHpBarPosition = Pixel::Vector2(15, 12);
	const Pixel::Vector2 bossHpBarPosition = Pixel::Vector2(25, 12);

	const int hpInterval = 1;
	const int hpWidth = 5;
	const int hpHeight = 2;

	bool isBossBeginAnimPlayed = false;
	bool isBossBegin = false;

	int currentPlayerHp = 0;
	int currentBossHp = 0;

	std::vector<std::string> hpBarSprite;

	std::vector<std::string> bossHpSprite;
	std::vector<std::string> playerHpSprite;

	OnFinished onFinished = nullptr;
};