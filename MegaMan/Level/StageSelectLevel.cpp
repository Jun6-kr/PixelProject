#include "StageSelectLevel.h"
#include <Game/GameManager.h>
#include <fileSystem>

StageSelectLevel::StageSelectLevel()
{
	LoadStageData();
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
	game.StartSelectedLevel(stageFolderPath);
}
