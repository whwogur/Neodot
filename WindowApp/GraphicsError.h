#pragma once
#include <source_location>

namespace Neodot::App
{
	struct CheckerToken {};
	extern CheckerToken checkToken;
	struct HrPipe
	{
		HrPipe(unsigned int hr, std::source_location = std::source_location::current()) noexcept;
		unsigned int m_hr;
		std::source_location m_location;
	};
	void operator>>(HrPipe, CheckerToken);
}