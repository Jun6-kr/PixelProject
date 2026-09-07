#pragma once
#include "LevelItem.h"
#include <Level/Level.h>
#include <unordered_map>


class StageSelectLevel : public Pixel::Level
{
	TYPE_DECLARATIONS(StageSelectLevel, Level)

	enum class LevelIndex
	{
		Airman = 0,
		Bubbleman,
		Clashman,
		Flashman,
		Heatman,
		Metalman,
		Quickman,
		Woodman
	};

	Pixel::Vector2 selctPosition[8] = { { 44, 18 }, { 115, 18 }, { 186, 18 }
	,{ 44, 62 }, { 186, 62 } ,{ 44, 106 } ,{ 115, 106 } ,{ 186, 106 } };

public:
	StageSelectLevel();
	~StageSelectLevel() = default;

	void LoadStageData();
	void SelectLevel();

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	void LoadStageMenuButtons();

private:
	int currentStageIndex = 0;
	std::vector<std::string> stageNames;
	std::unordered_map<std::string, std::string> stageList;

	std::vector<std::unique_ptr<MenuItem>> menues;
};