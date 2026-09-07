#include "GameManager.h"
#include <Level/TitleLevel.h>
#include <Level/StageSelectLevel.h>
#include <Level/GameLevel.h>
#include <Util/Util.h>

using namespace Pixel;

GameManager::GameManager()
{
	player = std::make_shared<Player>();

	player->SetAnimationClip(State::Idle, spriteManager.GetClip("Idle"));
	player->SetAnimationClip(State::Move, spriteManager.GetClip("Move"));
	player->SetAnimationClip(State::Attack, spriteManager.GetClip("Attack"));
	player->SetAnimationClip(State::Jump, spriteManager.GetClip("Jump"));
	player->SetAnimationClip(State::JumpAttack, spriteManager.GetClip("JumpAttack"));


	menuList.emplace_back(std::make_shared<TitleLevel>());
	menuList.emplace_back(std::make_shared<StageSelectLevel>());

	mainLevel = menuList[static_cast<int>(menuIndex)];

	//binder.Bind("", [this]()
	//	{
	//		StartSelectedLevel
	//	});
}

void GameManager::ShowSelectedLevel()
{
	menuIndex = MenuState::Select;
	nextLevel = menuList[static_cast<int>(menuIndex)];
}

void GameManager::StartSelectedLevel(const std::string& levelName)
{
	menuIndex = MenuState::InGame;
	auto game = std::make_shared<GameLevel>();
	game->SetInitData(levelName, player);
	menuList.emplace_back(game);
	mainLevel = menuList[static_cast<int>(menuIndex)];
	player->SetOwner(game);
}

void GameManager::TogglePauseMenu()
{
}

void GameManager::BackToTitle()
{
	menuIndex = MenuState::Title;
	mainLevel = menuList[static_cast<int>(menuIndex)];
}

void GameManager::FinishEffect(const Pixel::Vector2& position)
{
}

void GameManager::FinishGame()
{
}

const Pixel::AnimationClip* GameManager::GetClip(const std::string& key)
{
	return spriteManager.GetClip(key);
}
