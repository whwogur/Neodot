#include "EntryBuilder.h"
#include "Channel.h"
#include <Core/src/window/NeoWin.h>

#pragma warning(push) 
#pragma warning(disable: 26815) 
// Visual Studio static analysis; 함수에서 *this 반환 시 발생하는 경고 무시

namespace Neodot::Log
{
	EntryBuilder::EntryBuilder(const wchar_t* sourceFile, const wchar_t* sourceFunctionName, int sourceLine)
		: Entry{
			.m_sourceFile = sourceFile,
			.m_sourceFunctionName = sourceFunctionName,
			.m_sourceLine = sourceLine,
			.m_timestamp = std::chrono::system_clock::now()
		}
	{}

	EntryBuilder::~EntryBuilder()
	{
		if (m_pDest)
		{
			if ((static_cast<int>(m_level) <= static_cast<int>(Level::Error)))
			{
				m_trace.emplace(m_straceSkipDepth);
			}
			m_pDest->Submit(*this);
		}
	}

	EntryBuilder& EntryBuilder::note(std::wstring note)
	{
		m_note = std::move(note);
		return *this;
	}

	EntryBuilder& EntryBuilder::level(Level level)
	{
		m_level = level;
		return *this;
	}

	EntryBuilder& EntryBuilder::verbose(std::wstring note)
	{
		m_note = std::move(note);
		m_level = Level::Verbose;
		return *this;
	}
	EntryBuilder& EntryBuilder::debug(std::wstring note)
	{
		m_note = std::move(note);
		m_level = Level::Debug;
		return *this;
	}
	EntryBuilder& EntryBuilder::info(std::wstring note)
	{
		m_note = std::move(note);
		m_level = Level::Info;
		return *this;
	}
	EntryBuilder& EntryBuilder::warn(std::wstring note)
	{
		m_note = std::move(note);
		m_level = Level::Warn;
		return *this;
	}
	EntryBuilder& EntryBuilder::error(std::wstring note)
	{
		m_note = std::move(note);
		m_level = Level::Error;
		return *this;
	}
	EntryBuilder& EntryBuilder::fatal(std::wstring note)
	{
		m_note = std::move(note);
		m_level = Level::Fatal;
		return *this;
	}
	EntryBuilder& EntryBuilder::chan(IChannel* pChan)
	{
		m_pDest = pChan;
		return *this;
	}

	EntryBuilder& EntryBuilder::trace_skip(int depth)
	{
		m_straceSkipDepth = depth;
		return *this;
	}

	EntryBuilder& EntryBuilder::hr()
	{
		m_hResult = GetLastError();
		return *this;
	}

	EntryBuilder& EntryBuilder::hr(unsigned int hr)
	{
		m_hResult = hr;
		return *this;
	}
}
#pragma warning(pop)