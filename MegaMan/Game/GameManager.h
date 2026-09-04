#pragma once
#include "DebugCommandBinder.h"
#include <Engine\Engine.h>
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

class GameManager : public Pixel::Engine
{
public:
	GameManager();

	void ShowSelectedLevel();
	void StartSelectedLevel(const std::string& levelName);
	void TogglePauseMenu();
	void BackToTitle();

private:
	MenuState menuIndex = MenuState::Title;
	std::vector<std::shared_ptr<Pixel::Level>> menuList;

	// Debug
	DebugCommandBinder binder;
};