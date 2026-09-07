#include "Airman.h"
#include <Game/GameManager.h>

using namespace Pixel;

Airman::Airman(const Vector2& position) : Enemy(position)
{
	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	idleAnimtaion = game.GetClip("Airman_Idle");
	attackAnimtaion = game.GetClip("Airman_Attack");
	jumpAnimtaion = game.GetClip("Airman_Jump");

	HP = 28;
}

void Airman::OnDamaged(const int damage)
{
	if (HP < damage)
	{
		GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
		game.FinishEffect(position);
		SetActive(false);
	}

	HP -= damage;
}

void Airman::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// TODO : 보스 페이즈 진행

}

void Airman::Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd)
{
	Vector2 pos = position - screenOffset;

	switch (state)
	{
	case AirmanState ::Idle:
		idleAnimtaion->PlayAnimation(0, pos, screenEnd);
		break;
	case AirmanState ::Attack:
		if (attackIndex >= attackAnimtaion->GetSize())
		{
			attackIndex = 0;
		}
		attackAnimtaion->PlayAnimation(attackIndex, pos, screenEnd);
		++attackIndex;
		break;
	case AirmanState ::Jump:
		jumpAnimtaion->PlayAnimation(0, pos, screenEnd);
		break;

	}
}
