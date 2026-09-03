#pragma once
#include <Level/Level.h>

class TestLevel : public Pixel::Level
{
	TYPE_DECLARATIONS(TestLevel, Level)

public:
	TestLevel();


private:
	virtual void OnInitialized();
	virtual void BeginPlay();
	virtual void Tick(float deltaTime);
	virtual void Draw();
};

