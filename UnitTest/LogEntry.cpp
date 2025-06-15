#include "NeodotCppUnitTest.h"
#include <Core/src/log/EntryBuilder.h>
#include <Core/src/log/Channel.h>
#include <Core/src/log/Policy.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

// This macro makes Log Entrybuilder
#define neolog Neodot::Log::EntryBuilder{ __FILEW__, __FUNCTIONW__, __LINE__ }

class MockChannel : public Neodot::Log::IChannel
{
public:
	void Submit(Neodot::Log::Entry& e) override
	{
		m_entry = e;
	}
	void AttachDriver(std::shared_ptr<Neodot::Log::IDriver>) override {}
	virtual void AttachPolicy(std::shared_ptr<Neodot::Log::IPolicy>) override {}

	Neodot::Log::Entry m_entry;
};

// ENUM tostring
template<> inline std::wstring __cdecl
Microsoft::VisualStudio::CppUnitTestFramework::
ToString<Neodot::Log::Level>(const Neodot::Log::Level& level)
{
	return Neodot::Log::GetLevelName(level);
}

namespace Infrastructure::Log
{
	TEST_CLASS(LogTests)
	{
	public:
		// TEMP
		TEST_METHOD(LogEntryTests)
		{
			MockChannel chan;
			neolog.level(Neodot::Log::Level::Info).note(L"TEST").chan(&chan);
			Assert::AreEqual(L"TEST"s, chan.m_entry.m_note);
			Assert::AreEqual(Neodot::Log::Level::Info, chan.m_entry.m_level);
		}

		// testing simplified level/note
		TEST_METHOD(SimplifiedLevelNote)
		{
			MockChannel chan;
			neolog.info(L"TEST").chan(&chan);
			Assert::AreEqual(L"TEST"s, chan.m_entry.m_note);
			Assert::AreEqual(Neodot::Log::Level::Info, chan.m_entry.m_level);
		}
	};
}