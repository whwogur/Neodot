#include "NeodotCppUnitTest.h"
#include <Core/src/log/EntryBuilder.h>
#include <Core/src/log/Channel.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Neodot;
using namespace std::string_literals;

// This macro makes Log Entrybuilder
#define neolog Log::EntryBuilder{ __FILEW__, __FUNCTIONW__, __LINE__ }

class MockChannel : public Log::IChannel
{
public:
	void Submit(Log::Entry& e) override
	{
		m_entry = e;
	}
	Log::Entry m_entry;
};

// ENUM tostring
template<> inline std::wstring __cdecl
Microsoft::VisualStudio::CppUnitTestFramework::ToString<Log::Level>(const Log::Level& level)
{
	return Log::GetLevelName(level);
}

namespace Infrastructure
{
	TEST_CLASS(LogTests)
	{
	public:
		// TEMP
		TEST_METHOD(TempLogTest)
		{
			MockChannel chan;
			neolog.level(Log::Level::Info).note(L"TEST").chan(&chan);
			Assert::AreEqual(L"TEST"s, chan.m_entry.m_note);
			Assert::AreEqual(Log::Level::Info, chan.m_entry.m_level);
			Assert::AreEqual(39, chan.m_entry.m_sourceLine);
		}

		// testing simplified level/note
		TEST_METHOD(SimplifiedLevelNote)
		{
			MockChannel chan;
			neolog.info(L"TEST").chan(&chan);
			Assert::AreEqual(L"TEST"s, chan.m_entry.m_note);
			Assert::AreEqual(Log::Level::Info, chan.m_entry.m_level);
			Assert::AreEqual(49, chan.m_entry.m_sourceLine);
		}
	};
}