#pragma once
#include <Actor/Actor.h>

class HornObstacle : public Pixel::Actor
{
	TYPE_DECLARATIONS(HornObstacle, Actor)

public:
	HornObstacle(const Pixel::Vector2& position);
	~HornObstacle();

	virtual Pixel::Rect GetBounds() const override;

};

