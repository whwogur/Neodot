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
		oss << std::format(L"@{} {{{}}} {}\n  >> at {}\n     {}({})\n",
			GetLevelName(e.m_level),
			std::chrono::zoned_time{ std::chrono::current_zone(), e.m_timestamp },
			e.m_note,
			e.m_sourceFunctionName,
			e.m_sourceFile,
			e.m_sourceLine
		);

		if (e.m_hResult)
		{
			// HRESULT가 있는 경우 포맷된 문자열로 출력 (16진수 코드와 설명 포함)
			oss << std::format(L"  !HRESULT [{:#010x}]: {}\n", *e.m_hResult,
				Neodot::window::GetErrorDescription(*e.m_hResult));
		}

		if (e.m_trace)
		{
			oss << e.m_trace->Print() << std::endl;
		}

		return oss.str();
	}
}