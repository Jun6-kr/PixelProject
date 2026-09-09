#include "GameManager.h"
#include <Level/TitleLevel.h>
#include <Level/StageSelectLevel.h>
#include <Level/GameLevel.h>
#include <Input/Input.h>

using namespace Pixel;

GameManager::GameManager() : resetGauge(resetTime)
{
	SpawnPlayerCharacter();
	
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
	currentStage = levelName;

	auto game = std::make_shared<GameLevel>();
	game->SetInitData(currentStage, player);
	menuList.emplace_back(game);
	mainLevel = menuList[static_cast<int>(menuIndex)];
	player->SetOwner(game);
}

void GameManager::ResetGameLevel()
{
	menuIndex = MenuState::InGame;
	menuList[static_cast<int>(menuIndex)].reset();
	menuList.pop_back();

	auto game = std::make_shared<GameLevel>();
	player->Reset();
	game->SetInitData(currentStage, player);
	menuList.emplace_back(game);
	mainLevel = menuList[static_cast<int>(menuIndex)];
	player->SetOwner(game);
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

void GameManager::Tick(float deltaTime)
{
	Engine::Tick(deltaTime);

	if (Input::Get().GetKey('R'))
	{
		if (resetGauge <= 0.0f && isReleasedReset)
		{
			isReleasedReset = false;

			ResetGameLevel();
			resetGauge = resetTime;
		}
		else
		{
			resetGauge -= deltaTime;
		}
	}
	else if (Input::Get().GetKeyUp('R'))
	{
		isReleasedReset = true;
	}
}

void GameManager::SpawnPlayerCharacter()
{
	player = std::make_shared<Player>();

	player->SetAnimationClip(0, spriteManager.GetClip("Idle"));
	player->SetAnimationClip(1, spriteManager.GetClip("Move"));
	player->SetAnimationClip(2, spriteManager.GetClip("Attack"));
	player->SetAnimationClip(3, spriteManager.GetClip("Jump"));
	player->SetAnimationClip(4, spriteManager.GetClip("JumpAttack"));
}
