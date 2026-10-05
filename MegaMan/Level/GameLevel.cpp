#include "GameLevel.h"
#include <Game/GameManager.h>
#include <Actor/Player.h>
#include <Actor/Platform.h>
#include <Actor/Door.h>
#include <Actor/HornPlatform.h>
#include <Actor/AStarEnemy.h>
#include <Actor/AStarBullet.h>
#include <Actor/Enemy/Airman.h>
#include <Actor/Enemy/AirShooter.h>
#include <Actor/Enemy/Horn.h>

#include <Util/Util.h>
#include <Render/Renderer.h>
#include <Input/Input.h>
#include <Math/QuadTree.h>
#include <System/DebugSystem.h>	
#include <algorithm>

using namespace Pixel;
using namespace JxDebug;

GameLevel::GameLevel() : quadTree(new QuadTree())
{
	screenXMax = entryRoomCameraX;
}

GameLevel::~GameLevel()
{
	delete quadTree;
	quadTree = nullptr;
}

void GameLevel::SetInitData(const std::string& path, std::shared_ptr<Player> player)
{
	LoadLevelData(path);

	//binder.Bind("soft reset", [this]()
	//	{
	//		SoftReset();
	//	});
	//
	//binder.Bind("teleport boss", [this]()
	//	{
	//		TeleportBoss();
	//	});
	//
	//binder.Bind("boss phase2", [this]()
	//	{
	//		BossPhase2();
	//	});

	// HUD 설정
	player->BindDamageFunction([this](int hp)
		{
			hud.OnPlayerHpChanged(hp);
		});

	if (boss)
	{
		hud.OnBossHpAnimationFinished([this]()
			{
				GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
				game.GetPlayer().lock()->SetEnableInput(true);

				boss->StartPhase1();
			});
	}


	player->SetPosition(stageData.config.PlayerSpawnPos);
	addRequestedActorList.emplace_back(player);

	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	projectileManager.LoadProjectileDefinition(ProjectileType::RockBuster, game.GetClip("RockBuster"));
	projectileManager.LoadProjectileDefinition(ProjectileType::AirShooter, game.GetClip("AirShooter"));
	projectileManager.LoadProjectileDefinition(ProjectileType::Astar, game.GetClip("AStar"));

}

void GameLevel::UpdateCamera(float playerWorldX)
{
	if (isDoorTransition)
	{
		return;
	}

	const float followX = static_cast<float>(camEnd.x / 2);

	float playerScreenX = playerWorldX - screenOffsetX;

	if (playerScreenX > followX)
	{
		screenOffsetX = static_cast<int>(playerWorldX - followX);
	}

	screenOffsetX = std::clamp(screenOffsetX, 0, screenXMax - camEnd.x);
	//screenOffsetX = std::clamp(screenOffsetX, 0, stageData.config.stageWidth - camEnd.x);
}

CollisionResult GameLevel::CheckBlockCollision(Actor* requester, Rect rect)
{
	if (collisionMode == CollisionMode::BruteForce)
	{
		return CheckBlockCollisionBruteForce(requester, rect);
	}
	return CheckBlockCollisionQuadTree(requester, rect);
}

Pixel::CollisionResult GameLevel::CheckDamageCollision(Actor* requester, Pixel::Rect rect)
{
	if (collisionMode == CollisionMode::BruteForce)
	{
		return CheckDamageCollisionBruteForce(requester, rect);
	}
	return CheckDamageCollisionQuadTree(requester, rect);
}

Projectile* GameLevel::ShootProjectile(ProjectileType type, const Pixel::Vector2& position, const Pixel::Vector2& direction)
{
	return projectileManager.SpwnProjectile(type, position, direction, this);
}

void GameLevel::EnterBossEntryRoom()
{
	if (isDoorTransition)
		return;

	isDoorTransition = true;
	screenTargetX = entryRoomCameraX - 18;
	screenXMax = bossRoomCameraX;

	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	game.GetPlayer().lock()->SetEnableInput(false);
}

void GameLevel::EnterBossRoom()
{
	if (isDoorTransition)
		return;

	isDoorTransition = true;
	screenTargetX = bossRoomCameraX - 18;
	screenXMax = stageData.config.stageWidth;

	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	game.GetPlayer().lock()->SetEnableInput(false);
}

void GameLevel::OnInitialized()
{
	super::OnInitialized();
	SpawnProjectilePool();

	camStart = Vector2::Zero;
	camEnd = Vector2(Engine::Get().GetWidth(), Engine::Get().GetHeight());
}

void GameLevel::Tick(float deltaTime)
{
	UpdateActiveActors();

	super::Tick(deltaTime);

	hud.Tick(deltaTime);

	if (Input::Get().GetKeyDown('P'))
	{
		DebugSystem::Get().SetDebugMode(!DebugSystem::Get().IsDebugMode());
	}

	if (Input::Get().GetKeyDown('T'))
	{
		TeleportBoss();
	}

	if (Input::Get().GetKeyDown('O'))
	{
		collisionMode = (collisionMode == CollisionMode::BruteForce)
			? CollisionMode::QuadTree
			: CollisionMode::BruteForce;
	}

	if (isDoorTransition)
	{
		UpdateCameraTransition(deltaTime);
		return;
	}
}

void GameLevel::Draw()
{
	for (int index = 0; index < levelData.size(); ++index)
	{
		int x = 0;

		for (const char ch : levelData[index])
		{
			if (screenOffsetX <= x && x <= camEnd.x + screenOffsetX
				&& screenOffsetY <= index && index <= camEnd.y + screenOffsetY)
			{
				Renderer::Get().Submit(" ", Vector2(x - screenOffsetX, index - screenOffsetY), (ch != '*') ? ch : 0xFF, 0);
			}

			++x;
		}
	}

	for (std::shared_ptr<Actor>& actor : actorList)
	{
		// 검증 - 활성화 되지 않았으면 건너뛰기
		if (!actor->IsActive())
		{
			continue;
		}

		Vector2 screenOffset(screenOffsetX, screenOffsetY);
		actor->Draw(screenOffset, camEnd);
	}

	//for (const Rect& rect : quadTree->GetDebugRect())
	//{
	//	JxDebug::DebugSystem::Get().AddBox(rect.x - screenOffsetX, rect.y - screenOffsetY, rect.width, rect.height, 0x0D);
	//}
}

void GameLevel::LoadLevelData(const std::string& path)
{
	LoadLevelBackground(path + "_Background.txt");
	LoadLevelConfig(path + "_Config.txt");
	LoadActorList(path + "_Actors.txt");
	//LoadLevelActorData(path + "_Actors.txt");
}

void GameLevel::LoadLevelBackground(const std::string& path)
{
	Util::LoadTextFile(path, levelData);
}

void GameLevel::LoadLevelConfig(const std::string& configPath)
{
	StageConfig config = {};
	std::vector<std::string> lines;
	Util::LoadTextFile(configPath, lines);
	std::string str;

	for (const std::string line : lines)
	{
		if (line.empty() || (line.find_first_not_of(" \t") == std::string::npos))
		{
			continue;
		}

		if (line.front() == '[' && line.back() == ']')
		{
			str = line.substr(1, line.size() - 2);
			continue;
		}

		const size_t equalPosition = line.find('=');	// 문자열 내 = 위치 확인

		auto trim = [](std::string& s)
		{
			const char* whiteSpace = " \t\r\n";

			// 문자열의 앞에서부터 공백이 아닌 첫 문자 위치 검색
			const size_t begin = s.find_first_not_of(whiteSpace);

			if (begin == std::string::npos)
			{
				s.clear();
				return;
			}

			const size_t end = s.find_last_not_of(whiteSpace);
			s = s.substr(begin, end - begin + 1);
		};

		std::string key = line.substr(0, equalPosition);

		// value 파싱 - ' = ' 이후 전체
		std::string value = line.substr(equalPosition + 1);

		trim(key);
		trim(value);

		if (str == "Stage Size")
		{
			if (key == "width")
			{
				config.stageWidth = std::stoi(value);

			}
			else
			{
				config.stageHeight = std::stoi(value);
			}
		}
		else if (str == "Player Start")
		{
			if (key == "x")
			{
				config.PlayerSpawnPos.x = std::stoi(value);
			}
			else
			{
				config.PlayerSpawnPos.y = std::stoi(value);
			}
		}	
	}


	stageData.config = config;
}

void GameLevel::LoadActorList(const std::string& actorPath)
{
	std::vector<std::string> actors;
	Util::LoadTextFile(actorPath, actors);

	for (std::string& str : actors)
	{
		size_t start = 0;
		size_t pos = std::string::npos;

		if (str.find_first_not_of(" \t") == std::string::npos)
		{
			continue;
		}

		if (str[0] == '#')
		{
			continue;
		}

		int index = 0;
		ActorSpawnData data = { };

		while ((pos = str.find(',', start)) != std::string::npos)
		{ 
			std::string value = str.substr(start, pos - start);
			switch (index)
			{
			case 0:
				data.actorId = value;
				break;
			case 1:
				data.position.x = std::stoi(value);
				break;
			case 2:
				data.position.y = std::stoi(value);
				break;
			case 3:
				data.tileCountX = std::stoi(value);
				break;
			}

			start = pos + 1;
			++index;
		}

		data.tileCountY = std::stoi(str.substr(start));
		SpawnActorWithData(data);
		stageData.actors.emplace_back(data);
	}
}

void GameLevel::SetBlockedLocation(const Actor* spawned)
{
	Vector2 pos = spawned->GetPosition();
	const int width = spawned->GetWidth();
	const int height = spawned->GetHeight();

	for (int jx = 0; jx < height; ++jx)
	{
		pos.x = spawned->GetPosition().x;

		for (int ix = 0; ix < width; ++ix)
		{
			pos.x += 1;
			blockedPosition.insert(pos);
		}

		pos.y += 1;
	}
}

void GameLevel::SpawnActorWithData(const ActorSpawnData& data)
{
	Actor* spawned = nullptr;

	if (data.actorId == "Platform" || data.actorId == "Floor" || data.actorId == "Wall")
	{
		spawned = SpawnActor<Platform>(data.actorId, data.position, data.tileCountX, data.tileCountY).get();
	}
	else if (data.actorId == "HornPlatform")
	{
		spawned = SpawnActor<HornPlatform>(data.position).get();
	}
	else if (data.actorId == "Horn")
	{
		spawned = SpawnActor<Horn>(data.position).get();
	}
	else if (data.actorId == "AStarEnemy")
	{
		spawned = SpawnActor<AStarEnemy>(data.position).get();
	}
	else if (data.actorId == "Airman")
	{
		spawned = SpawnActor<Airman>(data.position).get();
		boss = dynamic_cast<Airman*>(spawned);
	}
	else if (data.actorId == "Door")
	{
		spawned = SpawnActor<Door>(data.position, data.tileCountY, data.position.x > entryRoomCameraX).get();
	}



	if (spawned)
	{
		SetBlockedLocation(spawned);
	}
}

void GameLevel::UpdateCameraTransition(float deltaTime)
{
	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	Player* player = game.GetPlayer().lock().get();

	screenOffsetX += static_cast<int>(transitionSpeed * deltaTime);
	player->UpdateFloatPosition(deltaTime * 0.45f);

	if (screenOffsetX >= screenTargetX)
	{
		screenOffsetX = screenTargetX;
		isDoorTransition = false;
		player->SetPosition(Vector2(player->GetPosition().x + 3, player->GetPosition().y));

		if (bossEntryState == BossEntryState::Stage)
		{
			player->SetEnableInput(true);
			bossEntryState = BossEntryState::EntryRoom;
		}
		else if (bossEntryState == BossEntryState::EntryRoom)
		{
			bossEntryState = BossEntryState::BossRoom;

			hud.OnAirmanPhaseBegan();
		}
	}
}

void GameLevel::UpdateActiveActors()
{
	Vector2 offsetStart(screenOffsetX, screenOffsetY);
	Rect screenView = Rect(camStart + offsetStart, camEnd + offsetStart);
	quadTree->ResetTree(screenView);

	for (const std::shared_ptr<Actor>& actor : actorList)
	{
		bool inView = screenView.Intersects(actor->GetBounds());

		if (inView)
		{
			actor->SetActive(true);
			quadTree->Insert(actor.get());
		}
		else if (!inView)
		{
			actor->SetActive(false);
		}
	}
}

CollisionResult GameLevel::CheckBlockCollisionBruteForce(const Actor* requester, Rect rect)
{
	CollisionResult result = {};

	// AABB로 검사 진행 이전 검사 대상 
	for (std::shared_ptr<Actor>& actor : actorList)
	{
		if (requester == actor.get())
		{
			continue;
		}

		Rect bound = actor->GetBounds();

		if (!rect.Intersects(bound))
		{
			continue;
		}

		uint32_t typeMask = static_cast<uint32_t>(requester->GetActorType());

		if (actor->GetBlockMask() & typeMask)
		{
			result.isBlocked = true;
			result.actor = actor.get();
			break;
		}
	}

	return result;
}

CollisionResult GameLevel::CheckDamageCollisionBruteForce(const Actor* requester, Rect rect)
{
	CollisionResult result = {};

	// AABB로 검사 진행 이전 검사 대상 
	for (std::shared_ptr<Actor>& actor : actorList)
	{
		if (requester == actor.get())
		{
			continue;
		}

		Rect bound = actor->GetBounds();

		if (!rect.Intersects(bound))
		{
			continue;
		}

		uint32_t typeMask = static_cast<uint32_t>(requester->GetActorType());

		if (actor->GetDamageMask() & typeMask)
		{
			result.isDamaged = true;
			result.actor = actor.get();
			break;
		}
	}

	return result;
}

CollisionResult GameLevel::CheckBlockCollisionQuadTree(Actor* requester, Rect rect)
{
	CollisionResult result = {};
	std::vector<Actor*> candidates = quadTree->Query(rect);

	for (Actor* candidate : candidates)
	{
		if (requester == candidate)
		{
			continue;
		}

		uint32_t typeMask = static_cast<uint32_t>(requester->GetActorType());

		if (candidate->GetBlockMask() & typeMask)
		{
			result.isBlocked = true;
			result.actor = candidate;

			candidate->OnCollision(requester);
			break;
		}
	}

	return result;
}

CollisionResult GameLevel::CheckDamageCollisionQuadTree(Actor* requester, Rect rect)
{
	CollisionResult result = {};
	std::vector<Actor*> candidates = quadTree->Query(rect);

	for (Actor* candidate : candidates)
	{
		if (requester == candidate)
		{
			continue;
		}

		uint32_t typeMask = static_cast<uint32_t>(requester->GetActorType());

		if (candidate->GetDamageMask() & typeMask)
		{
			result.isDamaged = true;
			result.actor = candidate;
			break;
		}
	}

	return result;
}

void GameLevel::SpawnProjectilePool()
{
	for (int ix = 0; ix < 32; ++ix)
	{
		std::shared_ptr<Projectile> projectile = SpawnActor<Projectile>();
		projectile->SetActive(false);
		projectileManager.RegisterProjectilePool(ProjectileType::RockBuster, projectile);
		
		if (ix < 6)
		{
			std::shared_ptr<Projectile> airshooter = SpawnActor<AirShooter>();
			airshooter->SetActive(false);
			projectileManager.RegisterProjectilePool(ProjectileType::AirShooter, airshooter);
		}

		if (ix < 10)
		{
			std::shared_ptr<Projectile> astar = SpawnActor<AStarBullet>();
			astar->SetActive(false);
			projectileManager.RegisterProjectilePool(ProjectileType::Astar, astar);
		}
	}
}

void GameLevel::TeleportBoss()
{
	bossEntryState = BossEntryState::EntryRoom;
	
	screenXMax = bossRoomCameraX;
	screenOffsetX = 600;
	screenOffsetY;

	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	game.GetPlayer().lock()->SetPosition(Vector2(660, 90));
}

void GameLevel::BossPhase2()
{
}
