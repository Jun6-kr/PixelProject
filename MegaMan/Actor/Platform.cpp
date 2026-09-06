#include "Platform.h"
#include <Render/Sprite.h>

using namespace Pixel;

Platform::Platform(const Vector2& position, int tileCountX) : Actor(position), tileCountX(tileCountX)
{
	actorType = ActorType::Terrain;
	blockMask = 0b11010;
	damageMask = 0b00000;

	for (int ix = 0; ix < tileCountX; ++ix)
	{
		sprites.emplace_back(new Sprite("../Assets/Actors/Platform/Basic/Tile_01.txt", actorType));
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

void Platform::Draw(const Vector2& camOffset, const Vector2& camEnd)
{
	int preSpriteWidth = 0;
	Vector2 pos = position - camOffset;

	for (Sprite*& sprite : sprites)
	{
		pos.x += preSpriteWidth;
		sprite->Draw(pos, camEnd);

		preSpriteWidth = sprite->GetSpriteWidth();
	}
}
