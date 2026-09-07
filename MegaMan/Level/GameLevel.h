#pragma once
#include <Level\Level.h>
#include <Game/ProjectileManager.h>
#include <Math/Rect.h>
#include <Physics/Collision.h>
#include <unordered_set>
#include <Game/DebugCommandBinder.h>

struct ActorSpawnData
{
	std::string actorId = "";
	Pixel::Vector2 position = Pixel::Vector2::Zero;
	int tileCountX = 0;
};

struct StageConfig
{
	int stageWidth = 0;
	int stageHeight = 0;
	Pixel::Vector2 PlayerSpawnPos = Pixel::Vector2::Zero;
};

struct StageData
{
	std::vector<ActorSpawnData> actors;
	StageConfig config;
};

namespace Pixel
{
	class Actor;
}

class Player;

class GameLevel : public Pixel::Level
{
	TYPE_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();

	void SetInitData(const std::string& path, std::shared_ptr<Player> player);

	void SpawnPlayer(std::shared_ptr<Player> player);

	void UpdateCamera(float playerWorldX);

	Pixel::CollisionResult CheckCollision(const Pixel::Actor* requester, Pixel::Rect rect);

	void ShootProjectile(ProjectileType type, const Pixel::Vector2& position, const Pixel::Vector2& direction);

	inline int& GetScreenOffsetX() { return screenOffsetX; }
	inline int& GetScreenOffsetY() { return screenOffsetY; }
	inline int& GetScreenWidth() { return camEnd.x; }
	inline int& GetScreenHeight() { return camEnd.y; }

	std::unordered_set<Pixel::Vector2>& GetBlocked() { return blockedPosition; }

protected:
	virtual void OnInitialized() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	void LoadLevelData(const std::string& path);
	void LoadLevelBackground(const std::string& path);
	void LoadLevelConfig(const std::string& configPath);
	
	void LoadActorList(const std::string& actorPath);
	void SetBlockedLocation(const Pixel::Actor* spawned);

	void SpawnActorWithData(const ActorSpawnData & data);

	void SpawnProjectilePool();

	void SoftReset();
	void TeleportBoss();
	void BossPhase2();

private:
	StageData stageData;

	std::unordered_set<Pixel::Vector2> blockedPosition;

	ProjectileManager projectileManager;

	DebugCommandBinder binder;

protected:
	// Setting.txt에서 지정
	Pixel::Vector2 camStart;
	Pixel::Vector2 camEnd;

	// 실시간 갱신
	int screenOffsetX = 0;
	int screenOffsetY = 0;
};