#pragma once
#include <Math/Vector2.h>
#include <string>

namespace Pixel
{
	enum class ActorType
	{
		None = 0,
		Terrain = 0b00001,
		Player = 0b00010,
		Enemy = 0b00100,
		RockBuster = 0b01000,
		Bullet = 0b10000,
	};
}