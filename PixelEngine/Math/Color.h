#pragma once

#include <Core/Core.h>
#include <Windows.h>

namespace Pixel
{
	// 색상을 열거형으로 정의
	enum class PIXEL_API Color : WORD
	{
		Outline = 0,
		White,
		MegaFace,
		MegaSuit,
		MegaSuitLight,
		DarkBlue,
		Yellow,
		LightCyan,
		DarkGary,
		LightGreen,
		Gray,
		Brown,

		DebugGreen = 0x0E,
		Transparent = 0xFF
	};
}