#include "TitleLevel.h"
#include <Util/Util.h>
#include <Render/Renderer.h>

using namespace Pixel;

TitleLevel::TitleLevel()
{
	Util::LoadTextFile("../Assets/Title.txt", levelData);
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
}
