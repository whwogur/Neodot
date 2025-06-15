#define _CRT_SECURE_NO_WARNINGS
#include "StackTrace.h"
#include <sstream>
#include "String.h"

#pragma warning(push)
#pragma warning(disable : 26495 26439 26451)
#include <Core/src/thirdparty/backward.hpp>
#pragma warning(pop)

namespace Neodot::util
{
	StackTrace::StackTrace(size_t skip)
	{
		backward::TraceResolver thisIsAWorkaround; // https://github.com/bombela/backward-cpp/issues/206
		m_pTrace = std::make_unique<backward::StackTrace>();
		m_pTrace->load_here(64);
		if (skip != 0)
		{
			m_pTrace->skip_n_firsts(skip);
		}
	}
	StackTrace::StackTrace(const StackTrace& src)
		: m_pTrace
		{ 
			src.m_pTrace ? 
			std::make_unique<backward::StackTrace>(*src.m_pTrace) : std::unique_ptr<backward::StackTrace>{} 
		}
	{
	}

	StackTrace::StackTrace(StackTrace&& donor) noexcept
		: m_pTrace{ std::move(donor.m_pTrace) }
	{
	}

	StackTrace& StackTrace::operator=(const StackTrace& src)
	{
		m_pTrace = src.m_pTrace ? std::make_unique<backward::StackTrace>(*src.m_pTrace) : std::unique_ptr<backward::StackTrace>{};
		return *this;
	}
	StackTrace& StackTrace::operator=(StackTrace&& donor) noexcept
	{
		if (&donor != this)
		{
			m_pTrace = std::move(donor.m_pTrace);
		}
		return *this;
	}

	// include backward.hpp in cpp file to remove bloat
	StackTrace::~StackTrace() {} // generate destructor where backward.hpp is at
	
	std::wstring StackTrace::Print() const
	{
		if (m_pTrace)
		{
				std::ostringstream oss;
			backward::Printer printer;
			printer.print(*m_pTrace, oss);
			return Neodot::util::ToWide(oss.str());
		}
		else
		{
			return L"== EMPTY TRACE ==";
		}
	}
}