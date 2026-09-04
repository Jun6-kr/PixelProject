#pragma once
#include <Core/Debug.h>
#include <string>
#include <unordered_map>
#include <functional>

namespace JxDebug
{
	//using Registered = void(*)();
	using Registered = std::function<void()>;

	class JX_DEBUG_API DebugCommander
	{
	private:
		DebugCommander() = default;
		
	public:
		~DebugCommander();

		static DebugCommander& Get();

		void Register(const std::string& key, Registered registed);
		void Unregister(const std::string& key);
		void Execute(const std::string& key);

	private:
		std::unordered_map<std::string, Registered> registedFunction;
	};
}