#include "StageSelectLevel.h"
#include <Input/Input.h>
#include <Game/GameManager.h>
#include <fileSystem>

using namespace Pixel;

StageSelectLevel::StageSelectLevel()
{
	LoadStageData();
	currentStageIndex = 0;
}

void StageSelectLevel::LoadStageData()
{
	const std::string stagePath = "../Assets/Stages/";

	for (const auto& entry : std::filesystem::directory_iterator(stagePath))
	{
		if (!entry.is_directory())
			continue;

		std::string stageName = entry.path().filename().string();
		std::string stageFolderPath = entry.path().string();

		stageList[stageName] = stageFolderPath;
		stageNames.emplace_back(stageName);
	}
}

void StageSelectLevel::SelectLevel()
{
	std::string stageFolderPath = stageList[stageNames[currentStageIndex]];

	GameManager& game = dynamic_cast<GameManager&>(Pixel::Engine::Get());
	game.StartSelectedLevel(stageFolderPath + "/" + stageNames[currentStageIndex]);
}

void StageSelectLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (Input::Get().GetKeyDown(VK_SPACE))
	{
		SelectLevel();
	}
}
