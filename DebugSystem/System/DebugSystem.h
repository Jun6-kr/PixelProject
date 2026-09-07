#pragma once
#include <Core/Debug.h>
#include <Observe/Observer.h>
#include <Visualization/Figure.h>
#include <Visualization/IDebugDraw.h>
#include <vector>

namespace JxDebug
{
	class JX_DEBUG_API DebugSystem
	{
	private:
		DebugSystem() = default;

	public:
		static DebugSystem& Get();

		inline void SetDebugMode(bool isDebug) { isDebugMode = isDebug; }
		inline bool IsDebugMode() const { return isDebugMode; }

		void AddPoint(const DebugPoint& point);
		void AddLine(const DebugLine& line);
		void AddBox(const DebugBox& box);
		void AddBox(const int x, const int y, const int width, const int height, const unsigned char colorIndex = 0x0E);

		void Draw(IDebugDraw* adapter);

		void ClearAllFigure();

		template<typename T>
		void Print(const std::string& valueName, const T& value)
		{
			observe.Print(valueName, value);
		}

		template<typename T>
		void Print(const std::string& valueName1, const T& value1, const std::string& valueName2, const T& value2)
		{
			observe.Print(valueName1, value1, valueName2, value2);
		}

		template<typename T>
		void DetailPrint(const std::string& valueName, const T& value, const std::source_location& source = std::source_location::current())
		{
			observe.DetailPrint(valueName, value, source);
		}

	private:
		bool isDebugMode = false;

		Observer observe;

		std::vector<DebugPoint> points;
		std::vector<DebugLine> lines;
		std::vector<DebugBox> boxes;
	};
}