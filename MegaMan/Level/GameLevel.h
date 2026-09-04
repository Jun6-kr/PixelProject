#pragma once
#include <Level\Level.h>
#include <Math/Rect.h>
#include <Physics/Collision.h>
//#include <unordered_map>
#include <Game/DebugCommandBinder.h>

//struct ActorDefinition
//{
//	std::string actorId = "";
//	std::vector<std::vector<Pixel::Sprite*>> sprites;
//};

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


class GameLevel : public Pixel::Level
{
	TYPE_DECLARATIONS(GameLevel, Level)

public:
	GameLevel(const std::string& path);

	void UpdateCamera(float playerWorldX);

	Pixel::CollisionResult CheckCollision(Pixel::Rect rect);

protected:
	virtual void OnInitialized() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	void LoadLevelData(const std::string& path);
	void LoadLevelBackground(const std::string& path);
	void LoadLevelConfig(const std::string& configPath);
	
	void LoadActorList(const std::string& actorPath);
	//std::vector<Pixel::Sprite*> GetSprite(const std::string& spritePath);
	void LoadLevelActorData(const std::string& actorPath);
	
	void SpawnActorWithData(const ActorSpawnData & data);

	void SoftReset();
	void TeleportBoss();
	void BossPhase2();

private:
	StageData stageData;
	//std::unordered_map<std::string, ActorDefinition> actorDefinitions;

	DebugCommandBinder binder;

protected:
	// Setting.txt에서 지정
	Pixel::Vector2 camStart;
	Pixel::Vector2 camEnd;

	// 실시간 갱신
	int screenOffsetX = 0;
	int screenOffsetY = 0;
};