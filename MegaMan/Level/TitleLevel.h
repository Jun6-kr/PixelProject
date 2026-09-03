#pragma once
#include <Level\Level.h>

class TitleLevel : public Pixel::Level
{
	TYPE_DECLARATIONS(TitleLevel, Level)

public:
	TitleLevel();
	~TitleLevel() = default;

private:
	virtual void Draw() override;
};

