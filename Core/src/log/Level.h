#pragma once
#include <string>

namespace Neodot::Log
{
	enum class Level
	{
		None,
		Fatal,
		Error,
		Warn,
		Info,
		Debug,
		Verbose,
	};

	std::wstring GetLevelName(Level);
}