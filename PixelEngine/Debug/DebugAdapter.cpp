#include "DebugAdapter.h"
#include <Render/Renderer.h>

using namespace JxDebug;
using namespace Pixel;

DebugAdapter* DebugAdapter::instance = nullptr;

DebugAdapter::DebugAdapter()
{
	instance = this;
}

DebugAdapter& DebugAdapter::Get()
{
	return *instance;
}

DebugBox DebugAdapter::MakeDebugBox(const Vector2& position, const int width, const int height, const unsigned char colorIndex)
{
	DebugBox box = {};
	box.position = GetDebugPoint(position);
	box.size = DebugPoint(static_cast<float>(width), static_cast<float>(height));
	box.colorIndex = colorIndex;

	return box;
}

void DebugAdapter::Draw2DPoint(const DebugPoint& point, unsigned char colorIndex)
{
	Renderer::Get().Submit(" ", GetVector(point), colorIndex, debugSortingOrder);
}

void DebugAdapter::Draw2DLine(const DebugLine& line, unsigned char colorIndex)
{
}

void DebugAdapter::Draw2DBox(const DebugBox& box, unsigned char colorIndex)
{
	int posX =static_cast<int>(box.position.x - 1);
	int posY =static_cast<int>(box.position.y - 1);

	const int width = posX + static_cast<int>(box.size.x + 2);
	const int height = posY + static_cast<int>(box.size.y + 2);

	for (int jx = posY; jx <height; ++jx)
	{
		for (int ix = posX; ix < width; ++ix)
		{
			if (ix != posX && ix != width - 1)
			{
				if (jx != posY && jx != height - 1)
				{
					continue;
				}
			}

			Renderer::Get().Submit(" ", Vector2(ix, jx), colorIndex, debugSortingOrder);
		}
	}
}

DebugPoint DebugAdapter::GetDebugPoint(const Vector2& vector)
{
	return DebugPoint(static_cast<float>(vector.x), static_cast<float>(vector.y));
}

Vector2 DebugAdapter::GetVector(const DebugPoint& debugVector)
{
	return Vector2(static_cast<int>(debugVector.x), static_cast<int>(debugVector.y));
}
