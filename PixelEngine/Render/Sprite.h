#pragma once
#include <Core/Core.h>
#include <Actor/ActorType.h>
#include <Math/Vector2.h>
#include <string>
#include <vector>

namespace Pixel
{
	class PIXEL_API Sprite
	{
	public:
		Sprite(const std::string& path, ActorType type);

		void Draw(const Vector2& startPos, const Vector2& boundary) const;

		std::vector<std::string>& GetSprite() { return sprite; }

		int GetSpriteWidth() const;
		int GetSpriteHeight() const { return static_cast<int>(sprite.size()); }

	private:
		std::vector<std::string> sprite;
		ActorType actorType;
	};
}



