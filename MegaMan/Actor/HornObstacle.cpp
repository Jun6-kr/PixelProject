#include "HornObstacle.h"
#include <Math/Rect.h>

using namespace Pixel;

HornObstacle::HornObstacle(const Vector2& position) : Actor(position)
{
}

HornObstacle::~HornObstacle()
{
}

Rect HornObstacle::GetBounds() const
{
    return Pixel::Rect(0);
}
