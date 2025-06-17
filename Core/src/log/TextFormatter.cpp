#include "TextFormatter.h"
#include "Entry.h"
#include <format>
#include <sstream>
#include <Core/src/window/Utilities.h>

namespace Neodot::Log
{
	std::wstring TextFormatter::Format(const Entry& e) const
	{
		std::wostringstream oss;
		oss << std::format(L"@{} {{{}}} {}",
			GetLevelName(e.m_level),
			std::chrono::zoned_time{ std::chrono::current_zone(), e.m_timestamp },
			e.m_note
		);

		if (e.m_hResult)
		{
			oss << std::format(L"\n  !HRESULT [{:#010x}]: {}", *e.m_hResult,
				Neodot::window::GetErrorDescription(*e.m_hResult));
		}

		if (e.m_bShowSourceLine.value_or(true))
		{
			oss << std::format(L"\n  >> at {}\n     {}({})\n",
				e.m_sourceFunctionName,
				e.m_sourceFile,
				e.m_sourceLine
			);
		}
		else
		{
			oss << "\n";
		}

		if (e.m_trace)
		{
			oss << e.m_trace->Print() << "\n";
		}

		return oss.str();
	}
}