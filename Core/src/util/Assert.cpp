#include "Assert.h"
#include <Core/src/Log/Log.h>

namespace Neodot::util
{
	namespace {
#ifdef NDEBUG
			constexpr int skip_depth = 3;
		constexpr int skip_depth_ex = 23;
#else
			constexpr int skip_depth = 7;
		constexpr int skip_depth_ex = 29;
#endif
	}

	Assertion::Assertion(std::wstring expression, const wchar_t* file, const wchar_t* function, int line, Consequence consequence)
		: m_file{ file }
		, m_function{ function }
		, m_line{ line }
		, m_consequence { consequence }
		, m_skipDepth { skip_depth }
	{
		m_stream << L"Assertion Failed! " << expression << "\n";
	}

	Assertion::~Assertion()
	{
		Neodot::Log::EntryBuilder{ m_file, m_function, m_line }
			.trace_skip(m_skipDepth)
			.chan(Neodot::Log::GetDefaultChannel())
			.level(Consequence::Terminate == m_consequence ? Neodot::Log::Level::Fatal : Neodot::Log::Level::Error)
			.note(m_stream.str());

		if (Consequence::Terminate == m_consequence)
		{
			Neodot::Log::GetDefaultChannel()->Flush();
			std::terminate();
		}
	}

	Assertion& Assertion::msg(const std::wstring& message)
	{
		m_stream << L"  Msg: " << message << L"\n";
		return *this;
	}

	void Assertion::ex()
	{
		m_consequence = Consequence::Exception;
		m_skipDepth = skip_depth_ex;
		throw FailedAssertion{};
	}
}