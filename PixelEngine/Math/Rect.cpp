#include "Rect.h"

using namespace Pixel;

Rect::Rect(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
{
}

Pixel::Rect::Rect(Vector2 start, Vector2 size) : x(static_cast<float>(start.x))
, y(static_cast<float>(start.y)), width(static_cast<float>(size.x)), height(static_cast<float>(size.y))
{
}
