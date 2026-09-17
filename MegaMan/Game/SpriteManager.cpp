#include "SpriteManager.h"

using namespace Pixel;

SpriteManager::SpriteManager()
{
	LoadDefaultSprite();

}

const AnimationClip* SpriteManager::GetClip(const std::string& key)
{
	return &spirtes[key];
}

void SpriteManager::LoadDefaultSprite()
{
	AnimationClip clip;
	clip.AddFrame(Sprite("../Assets/Player/Idle/Idle_1.txt", ActorType::Player));
	spirtes["Idle"] = clip;
	clip = {};

	clip.AddFrame(Sprite("../Assets/Player/Move/Move_1.txt", ActorType::Player));
	clip.AddFrame(Sprite("../Assets/Player/Move/Move_2.txt", ActorType::Player));
	clip.AddFrame(Sprite("../Assets/Player/Move/Move_3.txt", ActorType::Player));
	spirtes["Move"] = clip;
	clip = {};

	clip.AddFrame(Sprite("../Assets/Player/Attack/Attack.txt", ActorType::Player));
	spirtes["Attack"] = clip;
	clip = {};

	clip.AddFrame(Sprite("../Assets/Player/JumpAttack/JumpAttack.txt", ActorType::Player));
	spirtes["JumpAttack"] = clip;
	clip = {};

	clip.AddFrame(Sprite("../Assets/Player/Jump/Jump.txt", ActorType::Player));
	spirtes["Jump"] = clip;
	clip = {};

	// Platform
	clip.AddFrame(Sprite("../Assets/Actors/Platform/Basic/Platform.txt", ActorType::Terrain));
	spirtes["Platform"] = clip;
	clip = {};
	clip.AddFrame(Sprite("../Assets/Actors/Platform/Basic/Wall.txt", ActorType::Terrain));
	spirtes["Wall"] = clip;
	clip = {};
	clip.AddFrame(Sprite("../Assets/Actors/Platform/Basic/Floor.txt", ActorType::Terrain));
	spirtes["Floor"] = clip;
	clip = {};
	clip.AddFrame(Sprite("../Assets/Actors/Door/Door.txt", ActorType::Terrain));
	spirtes["Door"] = clip;
	clip = {};
	clip.AddFrame(Sprite("../Assets/Actors/Platform/Enemy/HornPlatform.txt", ActorType::Terrain));
	spirtes["HornPlatform"] = clip;
	clip = {};



	// Enemy
	clip.AddFrame(Sprite("../Assets/Actors/Enemy/Horn/Horn1.txt", ActorType::Enemy));
	clip.AddFrame(Sprite("../Assets/Actors/Enemy/Horn/Horn2.txt", ActorType::Enemy));
	clip.AddFrame(Sprite("../Assets/Actors/Enemy/Horn/Horn3.txt", ActorType::Enemy));
	clip.AddFrame(Sprite("../Assets/Actors/Enemy/Horn/Horn4.txt", ActorType::Enemy));
	clip.AddFrame(Sprite("../Assets/Actors/Enemy/Horn/Horn5.txt", ActorType::Enemy));
	clip.AddFrame(Sprite("../Assets/Actors/Enemy/Horn/Horn6.txt", ActorType::Enemy));
	clip.AddFrame(Sprite("../Assets/Actors/Enemy/Horn/Horn7.txt", ActorType::Enemy));
	spirtes["Horn"] = clip;
	clip = {};

	clip.AddFrame(Sprite("../Assets/Actors/Enemy/AStar/AStar.txt", ActorType::Enemy));
	spirtes["AStarEnemy"] = clip;
	clip = {};
	
	clip.AddFrame(Sprite("../Assets/Actors/Enemy/Airman/Idle/Airman.txt", ActorType::Enemy));
	spirtes["Airman_Idle"] = clip;
	clip = {};
	
	clip.AddFrame(Sprite("../Assets/Actors/Enemy/Airman/Wind/Airman1.txt", ActorType::Enemy));
	clip.AddFrame(Sprite("../Assets/Actors/Enemy/Airman/Wind/Airman2.txt", ActorType::Enemy));
	clip.AddFrame(Sprite("../Assets/Actors/Enemy/Airman/Wind/Airman3.txt", ActorType::Enemy));
	spirtes["Airman_Wind"] = clip;
	clip = {};
	
	clip.AddFrame(Sprite("../Assets/Actors/Enemy/Airman/AirShooter/Airman.txt", ActorType::Enemy));
	spirtes["Airman_Airshooter"] = clip;
	clip = {};
	
	
	//clip.AddFrame(Sprite("../Assets/Actors/Enemy/Airman/Jump/Airman.txt", ActorType::Enemy));
	//playerSpirtes["Airman_Jump"] = clip;
	//clip = {};


	// Bullet
	clip.AddFrame(Sprite("../Assets/Bullets/RockBuster/RockBuster.txt", ActorType::RockBuster));
	spirtes["RockBuster"] = clip;
	clip = {};

	clip.AddFrame(Sprite("../Assets/Bullets/AStar/AStar.txt", ActorType::Bullet));
	spirtes["AStar"] = clip;
	clip = {};

	clip.AddFrame(Sprite("../Assets/Bullets/AirShooter/AirShooter_1.txt", ActorType::Bullet));
	clip.AddFrame(Sprite("../Assets/Bullets/AirShooter/AirShooter_2.txt", ActorType::Bullet));
	clip.AddFrame(Sprite("../Assets/Bullets/AirShooter/AirShooter_2.txt", ActorType::Bullet));
	spirtes["AirShooter"] = clip;
	clip = {};

}
