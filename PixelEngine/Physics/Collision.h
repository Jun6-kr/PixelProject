#pragma once

namespace Pixel
{
	class Actor;

	enum class CollisionType
	{
		None,
		Block,
		Overlap,
		Damaged
	};

	enum class CollisionDirection
	{
		None = 0,
		Forward,
		Up,
		Backward,
		Down
	};

	struct CollisionResult
	{
		bool isDamaged = false;
		bool isBlocked = false;

		Actor* actor;
		CollisionDirection direction = CollisionDirection::None;
	};
}