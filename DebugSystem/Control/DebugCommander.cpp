#include "DebugCommander.h"

namespace JxDebug
{
	DebugCommander::~DebugCommander()
	{
		registedFunction.clear();
	}

	DebugCommander& DebugCommander::Get()
	{
		static DebugCommander instance;
		return instance;
	}

	void DebugCommander::Register(const std::string& key, Registered registed)
	{
		registedFunction.emplace(key, registed);
	}

	void DebugCommander::Unregister(const std::string& key)
	{
		registedFunction.erase(key);
	}

	void DebugCommander::Execute(const std::string& key)
	{
		auto iter = registedFunction.find(key);
		if (iter == registedFunction.end())
		{
			return;
		}

		Registered func = iter->second;
		func();
	}
}