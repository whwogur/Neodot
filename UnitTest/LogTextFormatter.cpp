#include "NeodotCppUnitTest.h"
#include <Core/src/log/Entry.h>
#include <Core/src/log/TextFormatter.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace Infrastructure::Log
{
	TEST_CLASS(LogTextFormatterTests)
	{
	public:
		// testing text formatting
		TEST_METHOD(TestFormat)
		{
			const Neodot::Log::Entry e{
				.m_level = Neodot::Log::Level::Info,
				.m_note = L"Hello World",
				.m_sourceFile = __FILEW__,
				.m_sourceFunctionName = __FUNCTIONW__,
				.m_sourceLine = __LINE__,
				.m_timestamp = std::chrono::system_clock::time_point{
					std::chrono::days{ 10'000 }
				}
			};
			Assert::AreEqual(
				L"@Info {1997-05-19 09:00:00.0000000 GMT+9} Hello World\n  >> at Infrastructure::Log::LogTextFormatterTests::TestFormat\n     C:\\Users\\cuteg\\Desktop\\Neodot\\UnitTest\\LogTextFormatter.cpp(21)\n"s,
				Neodot::Log::TextFormatter{}.Format(e)
			);
		}
	};
}