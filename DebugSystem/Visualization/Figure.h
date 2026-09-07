#pragma once

namespace JxDebug
{
	struct DebugPoint
	{
		float x = 0;
		float y = 0;
	};

	struct DebugLine
	{
		DebugPoint start;
		DebugPoint end;
	};

	struct DebugBox
	{
		DebugPoint position;
		DebugPoint size;

		unsigned char colorIndex;
	};
}