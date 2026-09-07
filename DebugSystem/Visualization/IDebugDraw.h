#pragma once
#include "Figure.h"
#include <Core/Debug.h>

namespace JxDebug
{
	class JX_DEBUG_API IDebugDraw
	{
	public:
		IDebugDraw() = default;

		virtual void Draw2DPoint(const DebugPoint& point, unsigned char colorIndex = 0x0E) = 0;
		virtual void Draw2DLine(const DebugLine& line, unsigned char colorIndex = 0x0E) = 0;
		virtual void Draw2DBox(const DebugBox& box, unsigned char colorIndex = 0x0E) = 0;
	};
}