#pragma once
#include "SpriteManager.h"
#include "DebugCommandBinder.h"
#include <Engine\Engine.h>
#include <Actor/Player.h>
#include <string>
#include <vector>

enum class MenuState
{
	Title = 0,
	Select,
	InGame,
	Pause,
	Result
};

namespace Pixel
{
	class AnimationClip;
}

class GameManager : public Pixel::Engine
{
public:
	GameManager();

	void ShowSelectedLevel();
	void StartSelectedLevel(const std::string& levelName);
	void TogglePauseMenu();
	void BackToTitle();

	void FinishEffect(const Pixel::Vector2& position);
	void FinishGame();

	const Pixel::AnimationClip* GetClip(const std::string& key);
	std::weak_ptr<Player> GetPlayer() const { return player; }

private:
	MenuState menuIndex = MenuState::Title;
	std::vector<std::shared_ptr<Pixel::Level>> menuList;

	SpriteManager spriteManager;
	std::shared_ptr<Player> player;

	// Debug
	DebugCommandBinder binder;
};