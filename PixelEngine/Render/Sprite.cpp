#include "Sprite.h"
#include <Util/Util.h>
#include <Render/Renderer.h>

using namespace Pixel;

Sprite::Sprite(const std::string& path, ActorType type) : actorType(type)
{
	Util::LoadTextFile(path, sprite);
}

void Sprite::Draw(const Vector2& position, const Vector2& startPos, const Vector2& endPos)
{
	Vector2 pos = position;
	 
	for (const std::string& str : sprite)
	{
		pos.x = position.x;

		for (const char ch : str)
		{
			if (startPos.x <= pos.x && startPos.y <= pos.y && pos.x <= endPos.x && pos.y <= endPos.y)
			{
				Renderer::Get().Submit(" ", pos, (ch != '*') ? ch : 0xFF, static_cast<int>(actorType));
			}

			pos.x += 1;
		}

		pos.y += 1;
	}
}

int Sprite::GetSpriteWidth()
{
	if (sprite.empty())
	{
		return 0;
	}

	return static_cast<int>(sprite[0].length());
}
