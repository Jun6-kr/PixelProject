#pragma once
#include <Core/Core.h>
#include <Math/Vector2.h>

namespace Pixel
{
	class PIXEL_API Rect
	{
	public:
		Rect(float left = 0, float top = 0, float width = 0, float height = 0);
		~Rect() = default;

		bool Compare(const Rect& other) const;

	public:
		float left = 0;
		float top = 0;
		float width = 0;
		float height = 0;
	};
}

