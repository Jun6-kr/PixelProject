#include "Rect.h"

using namespace Pixel;

Rect::Rect(float left, float top, float width, float height) : left(left), top(top), width(width), height(height)
{
}

Pixel::Rect::Rect(Vector2 start, Vector2 size) : left(static_cast<float>(start.x))
, top(static_cast<float>(start.y)), width(static_cast<float>(size.x)), height(static_cast<float>(size.y))
{
}

bool Rect::Compare(const Rect& other) const
{
    return left <= other.left + other.width
        && left + width >= other.left
        && top <= other.top + other.height
        && top + height >= other.top;
}
