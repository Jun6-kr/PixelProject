#pragma once
#include <Actor\Actor.h>
#include <vector>

namespace Pixel
{
	class Sprite;
}

class Platform : public Pixel::Actor
{
	TYPE_DECLARATIONS(Platform, Actor)

public:
	Platform(const Pixel::Vector2& position, int tileCountX);
	~Platform();

	virtual Pixel::Rect GetBounds() const override;

private:
	virtual void Draw(const Pixel::Vector2& camStart, const Pixel::Vector2& camEnd) override;
	
private:
	std::vector<Pixel::Sprite*> sprites;
	int width = 0;
	int height = 0;
	int tileCountX = 0;
};

