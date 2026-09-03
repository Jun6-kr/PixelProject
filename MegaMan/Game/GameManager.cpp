#include "GameManager.h"
#include <Input/Input.h>
#include <Level/TitleLevel.h>
#include <Level/StageSelectLevel.h>
#include <Level/GameLevel.h>

GameManager::GameManager()
{
	menuList.emplace_back(std::make_shared<TitleLevel>());
	menuList.emplace_back(std::make_shared<StageSelectLevel>());

	mainLevel = menuList[static_cast<int>(menuIndex)];
}

void GameManager::ShowSelectedLevel()
{
	menuIndex = MenuState::Select;
	nextLevel = menuList[static_cast<int>(menuIndex)];
	auto select = std::static_pointer_cast<StageSelectLevel>(nextLevel);
	
}

void GameManager::StartSelectedLevel(const std::string& levelName)
{
	menuIndex = MenuState::InGame;
	menuList.emplace_back(std::make_shared<GameLevel>(levelName));
	mainLevel = menuList[static_cast<int>(menuIndex)];
}

void GameManager::TogglePauseMenu()
{
}
