#pragma once
#include "Figure.h"
#include <Core/Debug.h>

namespace JxDebug
{
	class JX_DEBUG_API IDebugDraw
	{
	public:
		IDebugDraw() = default;

		virtual void Draw2DPoint(const DebugPoint& point) = 0;
		virtual void Draw2DLine(const DebugLine& line) = 0;
		virtual void Draw2DBox(const DebugBox& box) = 0;
	};
}