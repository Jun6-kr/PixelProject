#pragma once

namespace Pixel
{
	enum class ActorType
	{
		None = 0,
		RockBuster	= 0b00001,
		Bullet		= 0b00010,
		Terrain		= 0b00100,
		Enemy		= 0b01000,
		Player		= 0b10000,
	};
}