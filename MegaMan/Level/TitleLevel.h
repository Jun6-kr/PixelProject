#pragma once
#include <Level\Level.h>
#include <Level/LevelItem.h>

class TitleLevel : public Pixel::Level
{
	TYPE_DECLARATIONS(TitleLevel, Level)

public:
	TitleLevel();
	~TitleLevel() = default;

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	int currentMenuIndex = 0;
	std::vector<std::unique_ptr<MenuItem>> menuList;
};

