#include "TestLevel.h"
#include <Actor/Player.h>
//#include <Util/Util.h>

using namespace Pixel;

TestLevel::TestLevel()
{
	//Util::LoadTextFileWithSize("../Assets/TestFile.txt", levelData);

	//screenWidth = static_cast<int>(levelData[0].length());
	//screenHeight = static_cast<int>(levelData.size());
}

void TestLevel::OnInitialized()
{
	super::OnInitialized();

	SpawnActor<Player>(Vector2(30, 10));
}

void TestLevel::BeginPlay()
{
	super::BeginPlay();
}

void TestLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void TestLevel::Draw()
{
	super::Draw();
}
