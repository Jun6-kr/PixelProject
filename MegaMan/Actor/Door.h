#pragma once
#include <Actor/Actor.h>

class Door : public Pixel::Actor
{
	TYPE_DECLARATIONS(Door, Actor)

public:
	Door(const Pixel::Vector2& position);
	~Door();


};

