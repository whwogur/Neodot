#pragma once
#include <string>

namespace Neodot::Log
{
	enum class Level
	{
		Trace,
		Debug,
		Info,
		Warn,
		Error,
		Fatal,
	};

	std::wstring GetLevelName(Level);
}