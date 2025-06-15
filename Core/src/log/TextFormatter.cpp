#pragma once
#include "TextFormatter.h"
#include "Entry.h"
#include <format>
#include <sstream>

namespace Neodot::Log
{
	std::wstring TextFormatter::Format(const Entry& e) const
	{
		std::wostringstream oss;
		oss << std::format(L"@{} {{{}}} {}\n  >> at {}\n     {}({})\n",
			GetLevelName(e.m_level),
			std::chrono::zoned_time{ std::chrono::current_zone(), e.m_timestamp },
			e.m_note,
			e.m_sourceFunctionName,
			e.m_sourceFile,
			e.m_sourceLine
		);

		if (e.m_trace) {
			oss << e.m_trace->Print() << std::endl;
		}

		return oss.str();
	}
}