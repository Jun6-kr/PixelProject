#include "TitleLevel.h"
#include <Input/Input.h>
#include <Render/Renderer.h>
#include <Game/GameManager.h>

using namespace Pixel;

TitleLevel::TitleLevel()
{
	Util::LoadTextFile("../Assets/Title/Title.txt", levelData);

	pressStart = std::make_unique<MenuItem>("../Assets/Title/PressStart.txt", []()
		{
			GameManager& game = dynamic_cast<GameManager&>(Pixel::Engine::Get());
			game.ShowSelectedLevel();
		});
}

void TitleLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 메뉴 선택
	if (Input::Get().GetKeyDown(VK_SPACE) || Input::Get().GetKeyDown(VK_RETURN))
	{
		pressStart->onSelected();
	}
}

void TitleLevel::Draw()
{
	// 타이틀 그리기
	for (int y = 0; y < levelData.size(); ++y)
	{
		int x = 0;
		for (const char ch : levelData[y])
		{
			Renderer::Get().Submit(" ", Vector2(x, y), (ch != '*') ? ch : 0xFF);
			++x;
		}
	}

	int x = 45;
	int y = 101;
	for (const std::string& str : pressStart->menuText)
	{
		for (const char ch : str)
		{
			Renderer::Get().Submit(" ", Vector2(x, y), (ch != '*') ? ch : 0xFF);
			++x;
		}
		++y;
	}
}
