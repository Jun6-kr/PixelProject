#pragma once
#include <Control/DebugCommander.h>

class DebugCommandBinder
{
public:
	DebugCommandBinder();
	~DebugCommandBinder();

	void Bind(const std::string& key, JxDebug::Registered function);
	void UnbindAll();

private:
	std::vector<std::string> registeredKeys;
};

