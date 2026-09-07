#pragma once
#include <Core/Core.h>
#include <Visualization/IDebugDraw.h>
#include <Math/Vector2.h>

namespace Pixel
{
	class Sprite;
}

class PIXEL_API DebugAdapter : public JxDebug::IDebugDraw
{
public:
	DebugAdapter();
	~DebugAdapter() = default;
	
	static DebugAdapter& Get();

	JxDebug::DebugBox MakeDebugBox(const Pixel::Vector2& position, const int width, const int height, const unsigned char colorIndex = 0x0E);

	virtual void Draw2DPoint(const JxDebug::DebugPoint& point, unsigned char colorIndex = 0x0E) override;
	virtual void Draw2DLine(const JxDebug::DebugLine& line, unsigned char colorIndex = 0x0E) override;
	virtual void Draw2DBox(const JxDebug::DebugBox& box, unsigned char colorIndex = 0x0E) override;

	JxDebug::DebugPoint GetDebugPoint(const Pixel::Vector2& vector);
	Pixel::Vector2 GetVector(const JxDebug::DebugPoint& debugVector);

private:
	static DebugAdapter* instance;
	const int debugSortingOrder = 8;
};

