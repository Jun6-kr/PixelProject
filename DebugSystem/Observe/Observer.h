#pragma once
#include <Core/Debug.h>
#include <source_location>
#include <string>
#include <Windows.h>

class JX_DEBUG_API Observer
{
public:
	Observer() = default;

	template<typename T>
	void Print(const std::string& valueName, const T& value)
	{
		std::string str = "[" + valueName + " : " + ToDebugString(value) + "]\n";
		OutputDebugStringA(str.c_str());
	}

	template<typename T>
	void Print(const std::string& valueName1, const T& value1, const std::string& valueName2, const T& value2)
	{
		std::string str = "[" + valueName1 + " : " + ToDebugString(value1) + " / " + valueName2 + " : " + ToDebugString(value2) + "]\n";
		OutputDebugStringA(str.c_str());
	}

	template<typename T>
	void DetailPrint(const std::string& valueName, const T& value, const std::source_location& source)
	{
		std::string str = "---------------------------------------------\n";
		str += "file : ";
		str += source.file_name();
		str += "\nline : " + source.line();
		str += "\nfunction : ";
		str += source.function_name();
		str += "\n[" + valueName + " : " + ToDebugString(value) + "]\n";
		str += "---------------------------------------------\n";

		OutputDebugStringA(str.c_str());
	}

private:
	template<typename T>
	std::string ToDebugString(const T& value);

	template<>
	std::string ToDebugString<int>(const int& value)
	{
		return std::to_string(value);
	}

	template<>
	std::string ToDebugString<float>(const float& value)
	{
		return std::to_string(value);
	}

	template<>
	std::string ToDebugString<bool>(const bool& value)
	{
		return value ? "true" : "false";
	}

	//template<>
	//std::string ToDebugString<>(const bool& value)
	//{
	//	return value ? "true" : "false";
	//}
};
