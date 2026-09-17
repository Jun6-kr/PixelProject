#include "GameHUD.h"
#include <Render/Renderer.h>
#include <Util/Util.h>

using namespace Pixel;

GameHUD::GameHUD()
{
	Util::LoadTextFile("../Assets/HUD/HpBar.txt", hpBarSprite);
	Util::LoadTextFile("../Assets/HUD/Boss.txt", bossHpSprite);
	Util::LoadTextFile("../Assets/HUD/Player.txt", playerHpSprite);
}

void GameHUD::Tick(float deltaTime)
{
	DrawHPBar(playerHpSprite, playerHpBarPosition, currentPlayerHp);

	if (!isBossBegin)
	{
		return;
	}

	if (!isBossBeginAnimPlayed)
	{
		if (currentBossHp == 28)
		{
			isBossBeginAnimPlayed = true;
			onFinished();
		}
		else
		{
			currentBossHp += 1;
		}
	}

	DrawHPBar(bossHpSprite, bossHpBarPosition, currentBossHp);
}

void GameHUD::OnPlayerHpChanged(const int hp)
{
	currentPlayerHp = hp;
}

void GameHUD::OnAirmanHpChanged(const int hp)
{
	currentBossHp = hp;
}

void GameHUD::OnAirmanPhaseBegan()
{
	isBossBegin = true;
}

void GameHUD::DrawHPBar(const std::vector<std::string>& bar, const Pixel::Vector2& position, const int currentHp)
{
	Vector2 pos = position;

	int height = static_cast<int>(hpBarSprite.size());
	for (int iy = 0; iy < height; ++iy)
	{
		pos.x = position.x;

		int length = static_cast<int>(hpBarSprite[iy].length());
		for (int ix = 0; ix < length; ++ix)
		{
			Renderer::Get().Submit(" ", pos, hpBarSprite[iy][ix], 30);

			if (iy != 0 && iy != height - 1)
			{
				if (((height - 2) - currentHp < iy))
				{
					if (0 < ix && ix < length - 1)
					{
						Renderer::Get().Submit(" ", pos, bar[0][ix], 32);
					}
				}
			}

			pos.x += 1;
		}

		pos.y += 1;
	}
}
