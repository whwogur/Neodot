#pragma once
#include "Level.h"
#include <chrono>
#include <optional>
#include <Core/src/util/StackTrace.h>

namespace Neodot::Log
{
	struct Entry
	{
		Level m_level = Level::Error;
		std::wstring m_note = {};
		const wchar_t* m_sourceFile = nullptr;
		const wchar_t* m_sourceFunctionName = nullptr;
		int m_sourceLine = -1;
		std::chrono::system_clock::time_point m_timestamp = std::chrono::system_clock::time_point();
		std::optional<util::StackTrace> m_trace;
	};
}