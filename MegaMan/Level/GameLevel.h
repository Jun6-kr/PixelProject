#pragma once
#include <Level\Level.h>
#include <HUD/GameHUD.h>
#include <Game/ProjectileManager.h>
#include <Math/Rect.h>
#include <Physics/Collision.h>
#include <unordered_set>
#include <Game/DebugCommandBinder.h>

enum class BossEntryState
{
	Stage,
	EntryRoom,
	BossRoom
};

struct ActorSpawnData
{
	std::string actorId = "";
	Pixel::Vector2 position = Pixel::Vector2::Zero;
	int tileCountX = 0;
	int tileCountY = 0;
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
class Airman;

class GameLevel : public Pixel::Level
{
	TYPE_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();

	void SetInitData(const std::string& path, std::shared_ptr<Player> player);

	void SpawnPlayer(std::shared_ptr<Player> player);

	void UpdateCamera(float playerWorldX);

	Pixel::CollisionResult CheckCollision(const Pixel::Actor* requester, Pixel::Rect rect);

	Projectile* ShootProjectile(ProjectileType type, const Pixel::Vector2& position, const Pixel::Vector2& direction);

	inline int& GetScreenOffsetX() { return screenOffsetX; }
	inline int& GetScreenOffsetY() { return screenOffsetY; }
	inline int& GetScreenWidth() { return camEnd.x; }
	inline int& GetScreenHeight() { return camEnd.y; }

	std::unordered_set<Pixel::Vector2>& GetBlocked() { return blockedPosition; }

	void EnterBossEntryRoom();
	void EnterBossRoom();

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

	void UpdateCameraTransition(float deltaTime);

	void SpawnProjectilePool();

	void SoftReset();
	void TeleportBoss();
	void BossPhase2();

private:
	StageData stageData;
	ProjectileManager projectileManager;
	GameHUD hud;


	std::unordered_set<Pixel::Vector2> blockedPosition;


	bool isDoorTransition = false;
	BossEntryState bossEntryState = BossEntryState::Stage;
	int screenTargetX = 0;
	const float transitionSpeed = 100.f;

	int screenXMax = 0;

	const int entryRoomCameraX = 570 + 18;
	const int bossRoomCameraX = 835 + 18;

	Airman* boss = nullptr;

	DebugCommandBinder binder;

protected:
	// Setting.txt에서 지정
	Pixel::Vector2 camStart;
	Pixel::Vector2 camEnd;

	// 실시간 갱신
	int screenOffsetX = 0;
	int screenOffsetY = 0;
};