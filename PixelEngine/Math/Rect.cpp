#include "Rect.h"

using namespace Pixel;

Rect::Rect(float left, float top, float width, float height) : left(left), top(top), width(width), height(height)
{
}

bool Rect::Compare(const Rect& other) const
{
    return left <= other.left + other.width
        && left + width >= other.left
        && top <= other.top + other.height
        && top + height >= other.top;
}
