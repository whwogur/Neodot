#pragma once
#include <string>

namespace Neodot::util
{
	std::wstring ToWide(const std::string& narrow);
	std::string ToNarrow(const std::wstring& wide);
}