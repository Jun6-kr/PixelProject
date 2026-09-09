#include "GameHUD.h"
#include <Game/GameManager.h>
#include <Render/AnimationClip.h> 

using namespace Pixel;

GameHUD::GameHUD()
{
	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	HpBarSprite = game.GetClip("../Assets/HUD/HpBar.txt");
	BossHp =game.GetClip("../Assets/HUD/Boss.txt");
	PlayerHp = game.GetClip("../Assets/HUS/Player.txt");
}

void GameHUD::Tick(float deltaTime)
{
}

void GameHUD::OnPlayerDamaged(int hp, const int maxHp)
{
}
