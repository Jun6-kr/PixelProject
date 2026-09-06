#pragma once
#include "Sprite.h"
#include <Core/Core.h>

namespace Pixel
{
	class PIXEL_API AnimationClip
	{
	public:
		AnimationClip();

		void AddFrame(Pixel::Sprite sprite);
		const Pixel::Sprite& GetSprite(int index);

		void PlayAnimation(const int index, const Vector2& screenPos, const Vector2& boundary) const;

		const int GetSize() const { return static_cast<int>(sprites.size()); }

	private:
		std::vector<Pixel::Sprite> sprites;
	};
}

