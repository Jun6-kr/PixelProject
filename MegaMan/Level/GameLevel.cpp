#include "GameLevel.h"
#include <Game/GameManager.h>
#include <Actor/Player.h>
#include <Actor/Platform.h>
#include <Actor/HornObstacle.h>

#include <Util/Util.h>
#include <Render/Renderer.h>
#include <Input/Input.h>
#include <System/DebugSystem.h>	
#include <algorithm>

using namespace Pixel;
using namespace JxDebug;

GameLevel::GameLevel(const std::string& path, std::shared_ptr<Player> player)
{
	LoadLevelData(path);

	//stageData.config.stageWidth = static_cast<int>(levelData[0].length());
	//stageData.config.stageHeight = static_cast<int>(levelData.size());

	binder.Bind("soft reset", [this]()
		{
			SoftReset();
		});

	binder.Bind("teleport boss", [this]()
		{
			TeleportBoss();
		});

	binder.Bind("boss phase2", [this]()
		{
			BossPhase2();
		});

	player->UpdateFloatPosition(stageData.config.PlayerSpawnPos);
	addRequestedActorList.emplace_back(player);

	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	projectileManager.LoadProjectileDefinition(ProjectileType::RockBuster, game.GetClip("RockBuster"));
	projectileManager.LoadProjectileDefinition(ProjectileType::AirShooter, game.GetClip("AirShooter"));
	projectileManager.LoadProjectileDefinition(ProjectileType::Astar, game.GetClip("AStar"));
}

void GameLevel::UpdateCamera(float playerWorldX)
{
	const float followX = static_cast<float>(camEnd.x / 2);

	float playerScreenX = playerWorldX - screenOffsetX;

	if (playerScreenX > followX)
	{
		screenOffsetX = static_cast<int>(playerWorldX - followX);
	}

	screenOffsetX = std::clamp(screenOffsetX, 0, stageData.config.stageWidth - camEnd.x);
}

CollisionResult GameLevel::CheckCollision(const Actor* requester, Rect rect)
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

		if (!rect.Compare(bound))
		{
			continue;
		}

		uint32_t typeMask = static_cast<uint32_t>(requester->GetActorType());

		if (actor->GetBlockMask() & typeMask)
		{
			result.isBlocked = true;
		}

		if (actor->GetDamageMask() & typeMask)
		{
			result.isDamaged = true;
		}
	}

	return result;
}

void GameLevel::ShootProjectile(ProjectileType type, const Pixel::Vector2& position, bool isForward)
{
	projectileManager.SpwnProjectile(type, position, isForward);
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
	super::Tick(deltaTime);
	
	if (Input::Get().GetKeyDown('P'))
	{
		DebugSystem::Get().SetDebugMode(!DebugSystem::Get().IsDebugMode());
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

		Rect rect(Vector2(screenOffset.x - 30, screenOffset.y - 30), Vector2(camEnd.x + screenOffsetX, camEnd.y + screenOffsetY));
		Rect bound = actor->GetBounds();
		if (!bound.Compare(bound))
		{
			continue;
		}

		actor->Draw(screenOffset, camEnd);

		//// 1. World -> Screen
		//int screenLeft = bound.left - screenOffsetX;
		//int screenTop = bound.top - screenOffsetY;
		//int screenRight = screenLeft + bound.width;
		//int screenBottom = screenTop + bound.height;
		//
		//// 2. 화면 영역으로 Clip
		//int clippedLeft = screenLeft > 0 ? screenLeft : 0;
		//int clippedTop = screenTop > 0 ? screenTop : 0;
		//int clippedRight = screenRight < camEnd.x ? screenRight : camEnd.x;
		//int clippedBottom = screenBottom < camEnd.y ? screenBottom : camEnd.y;
		//
		//// 3. 최종 Screen 기준 Box
		//int clippedWidth = clippedRight - clippedLeft;
		//int clippedHeight = clippedBottom - clippedTop;
		//
		//JxDebug::DebugBox box = DebugAdapter::Get().MakeDebugBox(
		//	Vector2(clippedLeft, clippedTop), clippedWidth, clippedHeight);
		//JxDebug::DebugSystem::Get().AddBox(box);
	}
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

void GameLevel::LoadLevelActorData(const std::string& actorPath)
{
	std::vector<std::string> actors;
	Util::LoadTextFile(actorPath, actors);
	std::unordered_map<std::string, std::vector<std::string>> tempMap;
	std::string id = "";
	std::vector<std::string> sprites;

	for (std::string line : actors)
	{
		if (line[0] == '#')
		{
			continue;
		}

		if (line.empty() && !sprites.empty())
		{
			tempMap[id] = sprites;
			id = "";
		}

		if (line.front() == '[' && line.back() == ']')
		{
			id = line.substr(1, line.size() - 2);
			continue;
		}

		if (id != "")
		{
			const size_t equalPosition = line.find('=');	// 문자열 내 = 위치 확인

			auto trim = [](std::string& s)
				{
					const char* whiteSpace = " \t\r\n";

					// 문자열의 앞에서부터 공백이 아닌 첫 문자 위치 검색
					const size_t begin = s.find_first_not_of(whiteSpace);

					// 공백이 아닌 문자를 못찾은 경우에는 빈 문자열로 설정 후 변환
					if (begin == std::string::npos)
					{
						s.clear();
						return;
					}

					// 문자열의 뒤에서부터 공백이 아닌 마지막 문자 위치 검색
					const size_t end = s.find_last_not_of(whiteSpace);
					s = s.substr(begin, end - begin + 1);
				};

			// key 파싱
			std::string key = line.substr(0, equalPosition);

			// value 파싱 - ' = ' 이후 전체
			std::string value = line.substr(equalPosition + 1);

			// 람다 호출로 key/value에서 공백 제거
			trim(key);
			trim(value);

			if (key == "Sprite")
			{
				size_t start = 0, pos = 0;
				std::string str;
				while ((pos = value.find(',', start)) != std::string::npos)
				{
					str = value.substr(start, pos - start);
					trim(str);
					sprites.emplace_back(str);
					start = pos + 1;
				}

				trim(str);
				sprites.emplace_back(value.substr(start));
			}
		}
	}

	//for (const auto& [actorId, spriteIds] : tempMap)
	//{
	//	ActorDefinition def = { };
	//	def.actorId = actorId;
	//
	//	for (const std::string& sprite : spriteIds)
	//	{
	//		std::string str = "../Assets/Actors/" + actorId + "/" + sprite;
	//
	//		def.sprites.emplace_back(GetSprite(sprite));
	//	}
	//}
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
			}

			start = pos + 1;
			++index;
		}

		data.tileCountX = std::stoi(str.substr(start));
		SpawnActorWithData(data);
		stageData.actors.emplace_back(data);
	}
}

void GameLevel::SpawnActorWithData(const ActorSpawnData& data)
{
	if (data.actorId == "Platform")
	{
		SpawnActor<Platform>(data.position, data.tileCountX);
	}
	else if (data.actorId == "HornObstacle")
	{
		SpawnActor<HornObstacle>(data.position);
	}
}

void GameLevel::SpawnProjectilePool()
{
	for (int ix = 0; ix < 32; ++ix)
	{
		std::shared_ptr<Projectile> projectile = SpawnActor<Projectile>();
		projectile->SetActive(false);
		projectileManager.RegisterProjectilePool(projectile);
	}
}

void GameLevel::SoftReset()
{
}

void GameLevel::TeleportBoss()
{
}

void GameLevel::BossPhase2()
{
}
