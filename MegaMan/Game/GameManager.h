#pragma once
#include <Engine\Engine.h>
#include <Actor/ActorType.h>
#include <string>
#include <vector>

struct ActorSpawnData
{
	Pixel::ActorType actorType = Pixel::ActorType::None;
	std::string actorName = "";
	Pixel::Vector2 position = Pixel::Vector2::Zero;
	int tileXCount = 0;
};

struct StageConfig
{
	int stageWidth = 0;
	int stageHeight = 0;
	Pixel::Vector2 PlayerSpawnPos = Pixel::Vector2::Zero;
};

struct StageData
{
	std::vector<std::string> background;
	std::vector<ActorSpawnData> actors;
	StageConfig config;
};


enum class MenuState
{
	Title = 0,
	Select,
	Pause,
	InGame,
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

};