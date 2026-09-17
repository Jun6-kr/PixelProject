#pragma once
#include <Core/Core.h>
#include <Math/Vector2.h>

namespace Pixel
{
	class PIXEL_API Rect
	{
	public:
		Rect(float x = 0, float y = 0, float width = 0, float height = 0);
		Rect(Vector2 start, Vector2 size);
		~Rect() = default;

		int GetX() const { return static_cast<int>(x); };
		int GetY() const { return static_cast<int>(y); };
		int GetWidth() const { return static_cast<int>(width); };
		int GetHeight() const { return static_cast<int>(height); };

		int GetXMax() const { return static_cast<int>(x + width); }
		int GetYMax() const { return static_cast<int>(y + height); }

		bool Contains(const int pointX, const int pointY) const
		{
			return pointX >= x && pointX < GetXMax()
				&& y <= pointY && pointY < GetYMax();
		}
		bool Contains(const Rect& other) const
		{
			return x <= other.x && other.GetXMax() < GetXMax()
				&& y <= other.y && other.GetYMax() < GetYMax();
		}
		bool Intersects(const Rect& other) const
		{
			return other.x <= GetXMax() && x < other.GetXMax()
				&& other.y <= GetYMax() && y < other.GetYMax();
		}

	public:
		float x = 0;
		float y = 0;

		float width = 0;
		float height = 0;
	};
}

