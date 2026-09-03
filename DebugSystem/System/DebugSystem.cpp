#include "DebugSystem.h"

using namespace JxDebug;

DebugSystem& JxDebug::DebugSystem::Get()
{
	static DebugSystem instance;
	return instance;
}

void DebugSystem::AddPoint(const DebugPoint& point)
{
	points.emplace_back(point);
}

void DebugSystem::AddLine(const DebugLine& line)
{
	lines.emplace_back(line);
}

void DebugSystem::AddBox(const DebugBox& box)
{
	boxes.emplace_back(box);
}

void DebugSystem::Draw(IDebugDraw* adapter)
{
	if (!isDebugMode)
		return;

	for (const DebugPoint& point : points)
	{
		adapter->Draw2DPoint(point);
	}

	for (const DebugLine& point : lines)
	{
		adapter->Draw2DLine(point);
	}

	for (const DebugBox& point : boxes)
	{
		adapter->Draw2DBox(point);
	}
}

void JxDebug::DebugSystem::ClearAllFigure()
{
	points.clear();
	lines.clear();
	boxes.clear();
}
