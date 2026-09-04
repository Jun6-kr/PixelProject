#include "DebugCommandBinder.h"

using namespace JxDebug;

DebugCommandBinder::DebugCommandBinder()
{
}

DebugCommandBinder::~DebugCommandBinder()
{
	UnbindAll();
}

void DebugCommandBinder::Bind(const std::string& key, JxDebug::Registered function)
{
	registeredKeys.emplace_back(key);
	DebugCommander::Get().Register(key, function);
}

void DebugCommandBinder::UnbindAll()
{
	for (const std::string& key : registeredKeys)
	{
		DebugCommander::Get().Unregister(key);
	}
}
