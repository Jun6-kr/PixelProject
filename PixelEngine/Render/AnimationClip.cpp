#include "AnimationClip.h"

namespace Pixel
{
	AnimationClip::AnimationClip()
	{}

	void AnimationClip::AddFrame(Pixel::Sprite sprite)
	{
		sprites.emplace_back(std::move(sprite));
	}

	const Pixel::Sprite& AnimationClip::GetSprite(int index)
	{
		return sprites[index];
	}

	void AnimationClip::PlayAnimation(const int index, const Vector2& screenPos, const Vector2& boundary) const
	{
		if (sprites.empty() || index >= sprites.size())
		{
			return;
		}

		sprites[index].Draw(screenPos, boundary);
	}
}
