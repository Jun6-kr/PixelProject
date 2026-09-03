#include <Level/GameLevel.h>
#include <Engine/Engine.h>
//#include <Game/GameManager.h>

int main()
{
	Pixel::Engine engine;
	//GameManager engine;
	engine.AddNewLevel<GameLevel>();
	engine.Run();
}