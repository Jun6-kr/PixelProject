#include "StageSelectLevel.h"
#include <Input/Input.h>
#include <Game/GameManager.h>
#include <Render/Renderer.h>
#include <Util/Util.h>
#include <fileSystem>

using namespace Pixel;

StageSelectLevel::StageSelectLevel()
{
	Util::LoadTextFile("../Assets/Title/StageSelect.txt", levelData);

	LoadStageData();
	currentStageIndex = 0;

	LoadStageMenuButtons();
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
	std::string stageFolderPath = stageList[stageNames[/*currentStageIndex*/0]];

	GameManager& game = dynamic_cast<GameManager&>(Pixel::Engine::Get());
	game.StartSelectedLevel(stageFolderPath + "/" + stageNames[/*currentStageIndex*/0]);
}

void StageSelectLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (Input::Get().GetKeyDown(VK_UP))
	{
		switch (currentStageIndex)
		{
		case 0:
		case 1:
		case 2:
			currentStageIndex += 5;
			break;
		case 3:
		case 7:
			currentStageIndex -= 3;
			break;
		case 4:
		case 5:
			currentStageIndex -= 2;
			break;
		case 6:
			currentStageIndex = 1;
			break;
		}
	}
	else if (Input::Get().GetKeyDown(VK_DOWN))
	{
		switch (currentStageIndex)
		{
		case 0:
		case 4:
			currentStageIndex += 3;
			break;
		case 2:
		case 3:
			currentStageIndex += 2;
			break;
		case 5:
		case 6:
		case 7:
			currentStageIndex -= 5;
			break;
		case 1:
			currentStageIndex = 6;
			break;
		}
	}
	else if (Input::Get().GetKeyDown(VK_LEFT))
	{
		--currentStageIndex;
		if (currentStageIndex < 0)
		{
			currentStageIndex = static_cast<int>(menues.size()) - 1;
		}
	}
	else if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		++currentStageIndex;
		if (currentStageIndex >= menues.size())
		{
			currentStageIndex = 0;
		}
	}

	if (Input::Get().GetKeyDown(VK_SPACE) || Input::Get().GetKeyDown(VK_RETURN))
	{
		SelectLevel();
	}

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		currentStageIndex = 0;
		GameManager& game = dynamic_cast<GameManager&>(Pixel::Engine::Get());
		game.BackToTitle();
	}
}

void StageSelectLevel::Draw()
{
	for (int y = 0; y < levelData.size(); ++y)
	{
		int x = 0;
		for (const char ch : levelData[y])
		{
			Renderer::Get().Submit(" ", Vector2(x, y), (ch != '*') ? ch : 0xFF);
			++x;
		}
	}

	for (int index = 0; index < menues.size(); ++index)
	{
		for (int line = 0; line < menues[index]->menuText.size(); ++line)
		{
			int x = selctPosition[index].x;
	
			for (const char ch : menues[index]->menuText[line])
			{
				Renderer::Get().Submit(" ", Vector2(x, selctPosition[index].y + line), (ch != '@') ? ch : (currentStageIndex == index ? 5 : 4));
				++x;
			}
		}
	}
}

void StageSelectLevel::LoadStageMenuButtons()
{
	auto airman = [this]()
		{
			SelectLevel();
		};

	menues.emplace_back(std::make_unique<MenuItem>("../Assets/Title/Menues/Select/Airman.txt", airman));
	menues.emplace_back(std::make_unique<MenuItem>("../Assets/Title/Menues/Select/Bubbleman.txt", airman));
	menues.emplace_back(std::make_unique<MenuItem>("../Assets/Title/Menues/Select/Clashman.txt", airman));
	menues.emplace_back(std::make_unique<MenuItem>("../Assets/Title/Menues/Select/Flashman.txt", airman));
	menues.emplace_back(std::make_unique<MenuItem>("../Assets/Title/Menues/Select/Heatman.txt", airman));
	menues.emplace_back(std::make_unique<MenuItem>("../Assets/Title/Menues/Select/Metalman.txt", airman));
	menues.emplace_back(std::make_unique<MenuItem>("../Assets/Title/Menues/Select/Quickman.txt", airman));
	menues.emplace_back(std::make_unique<MenuItem>("../Assets/Title/Menues/Select/Woodman.txt", airman));
}
