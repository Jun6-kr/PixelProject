#include "Platform.h"
#include <Render/Sprite.h>

using namespace Pixel;

Platform::Platform(const Vector2& position, int tileCountX) : Actor(position), tileCountX(tileCountX)
{
	actorType = ActorType::Terrain;

	for (int ix = 0; ix < tileCountX; ++ix)
	{
		sprites.emplace_back(new Sprite("../Assets/Platform/Tile_01.txt", actorType));
	}

	width = sprites[0]->GetSpriteWidth() * tileCountX;
	height = sprites[0]->GetSpriteHeight();
}

Platform::~Platform()
{
	for (Sprite*& sprite : sprites)
	{
		delete sprite;
		sprite = nullptr;
	}
}

Rect Platform::GetBounds() const
{
	return Rect(static_cast<float>(position.x)
		, static_cast<float>(position.y)
		, static_cast<float>(width)
		, static_cast<float>(height));
}

void Platform::Draw(const Vector2& camStart, const Vector2& camEnd)
{
	int preSpriteWidth = 0;

	for (Sprite*& sprite : sprites)
	{
		Vector2 pos = position;
		pos.x += preSpriteWidth;
		sprite->Draw(pos, camStart, camEnd);

		preSpriteWidth = sprite->GetSpriteWidth();
	}
}
