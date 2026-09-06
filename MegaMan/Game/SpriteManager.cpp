#include "SpriteManager.h"

using namespace Pixel;

SpriteManager::SpriteManager()
{
	LoadDefaultSprite();

}

const AnimationClip* SpriteManager::GetClip(const std::string& key)
{
	return &playerSpirtes[key];
}

void SpriteManager::LoadDefaultSprite()
{
	AnimationClip clip;
	clip.AddFrame(Sprite("../Assets/Player/Idle/Idle_1.txt", ActorType::Player));
	playerSpirtes["Idle"] = clip;
	clip = {};

	clip.AddFrame(Sprite("../Assets/Player/Move/Move_1.txt", ActorType::Player));
	clip.AddFrame(Sprite("../Assets/Player/Move/Move_2.txt", ActorType::Player));
	clip.AddFrame(Sprite("../Assets/Player/Move/Move_3.txt", ActorType::Player));
	playerSpirtes["Move"] = clip;
	clip = {};

	clip.AddFrame(Sprite("../Assets/Player/Attack/Attack.txt", ActorType::Player));
	playerSpirtes["Attack"] = clip;
	clip = {};

	clip.AddFrame(Sprite("../Assets/Player/JumpAttack/JumpAttack.txt", ActorType::Player));
	playerSpirtes["JumpAttack"] = clip;
	clip = {};

	clip.AddFrame(Sprite("../Assets/Player/Jump/Jump.txt", ActorType::Player));
	playerSpirtes["Jump"] = clip;
	clip = {};


	// Bullet
	clip.AddFrame(Sprite("../Assets/Bullets/RockBuster/RockBuster.txt", ActorType::RockBuster));
	playerSpirtes["RockBuster"] = clip;
	clip = {};

	clip.AddFrame(Sprite("../Assets/Bullets/AStar/AStar.txt", ActorType::Bullet));
	playerSpirtes["AStar"] = clip;
	clip = {};

	clip.AddFrame(Sprite("../Assets/Bullets/AirShooter/AirShooter_1.txt", ActorType::Bullet));
	clip.AddFrame(Sprite("../Assets/Bullets/AirShooter/AirShooter_2.txt", ActorType::Bullet));
	playerSpirtes["AirShooter"] = clip;
	clip = {};
}
