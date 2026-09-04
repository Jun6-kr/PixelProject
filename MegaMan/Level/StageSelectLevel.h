#pragma once
#include <Level/Level.h>
#include <unordered_map>

class StageSelectLevel : public Pixel::Level
{
	TYPE_DECLARATIONS(StageSelectLevel, Level)

public:
	StageSelectLevel();
	~StageSelectLevel() = default;

	void LoadStageData();
	void SelectLevel();

private:
	virtual void Tick(float deltaTime) override;

private:
	int currentStageIndex = 0;
	std::vector<std::string> stageNames;
	std::unordered_map<std::string, std::string> stageList;
};