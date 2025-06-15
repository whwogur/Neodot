#pragma once
#include "TextFormatter.h"
#include "Entry.h"
#include <format>

namespace Neodot::Log
{
	std::wstring TextFormatter::Format(const Entry& e) const
	{
		return std::format(L"@{} {{{}}} {}\n  >> at {}\n     {}({})\n",
			GetLevelName(e.m_level),
			std::chrono::zoned_time{ std::chrono::current_zone(), e.m_timestamp },
			e.m_note,
			e.m_sourceFunctionName,
			e.m_sourceFile,
			e.m_sourceLine
		);
	}
}