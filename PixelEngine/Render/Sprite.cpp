#include "Sprite.h"
#include <Util/Util.h>
#include <Render/Renderer.h>

using namespace Pixel;

Sprite::Sprite(const std::string& path, ActorType type) : actorType(type)
{
	Util::LoadTextFile(path, sprite);
}

void Sprite::Draw(const Vector2& startPos, const Vector2& boundary)
{
	int xPos = startPos.x;
	int yPos = startPos.y;
	
	for (const std::string& str : sprite)
	{
		xPos = startPos.x;

		// 시작 지점이 화면을 넘어가면 그리지 않는다.
		if (yPos < 0 || yPos > boundary.y)
		{
			yPos += 1;
			continue;
		}

		for (const char ch : str)
		{
			// 시작 지점이 화면을 넘어가면 그리지 않는다.
			if (xPos < 0 || xPos > boundary.x)
			{
				xPos += 1;
				continue;
			}

			Renderer::Get().Submit(" ", Vector2(xPos, yPos), (ch != '*') ? ch : 0xFF, static_cast<int>(actorType));
		
			xPos += 1;
		}

		yPos += 1;
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
