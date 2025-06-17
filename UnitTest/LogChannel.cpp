#include "NeodotCppUnitTest.h"
#include <Core/src/Log/EntryBuilder.h>
#include <Core/src/Log/Channel.h>
#include <Core/src/Log/Driver.h>
#include <Core/src/Log/SeverityLevelPolicy.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

#define neolog Neodot::Log::EntryBuilder{ __FILEW__, __FUNCTIONW__, __LINE__ }

class MockDriver : public Neodot::Log::IDriver
{
public:
	void Submit(const Neodot::Log::Entry& e) override
	{
		m_entry = e;
	}
	void Flush() override {}

	Neodot::Log::Entry m_entry;
};

template<> inline std::wstring __cdecl
Microsoft::VisualStudio::CppUnitTestFramework::
ToString<Neodot::Log::Level>(const Neodot::Log::Level& level)
{
	return Neodot::Log::GetLevelName(level);
}

namespace Infrastructure::Log
{
	TEST_CLASS(LogChannelTests)
	{
	public:
		// test channel forwarding entries to driver
		TEST_METHOD(TestForwarding)
		{
			Neodot::Log::Channel chan;
			std::shared_ptr<MockDriver> pDriver1 = std::make_shared<MockDriver>();
			std::shared_ptr<MockDriver> pDriver2 = std::make_shared<MockDriver>();
			chan.AttachDriver(pDriver1);
			chan.AttachDriver(pDriver2);
			neolog.info(L"HI").chan(&chan);
			Assert::AreEqual(L"HI"s, pDriver1->m_entry.m_note);
			Assert::AreEqual(Neodot::Log::Level::Info, pDriver1->m_entry.m_level);
			Assert::AreEqual(L"HI"s, pDriver2->m_entry.m_note);
			Assert::AreEqual(Neodot::Log::Level::Info, pDriver2->m_entry.m_level);
		}

		// test channel policy filtering
		TEST_METHOD(TestPolicyFiltering)
		{
			Neodot::Log::Channel chan;
			std::shared_ptr<MockDriver> pDriver1 = std::make_shared<MockDriver>();
			chan.AttachDriver(pDriver1);
			chan.AttachPolicy(std::make_unique<Neodot::Log::SeverityLevelPolicy>(Neodot::Log::Level::Info));
			neolog.info(L"HI").chan(&chan);
			Assert::AreEqual(L"HI"s, pDriver1->m_entry.m_note);
			Assert::AreEqual(Neodot::Log::Level::Info, pDriver1->m_entry.m_level);

			neolog.debug(L"Hello World").chan(&chan);
			Assert::AreEqual(L"HI"s, pDriver1->m_entry.m_note);
			Assert::AreEqual(Neodot::Log::Level::Info, pDriver1->m_entry.m_level);
		}
	};
}