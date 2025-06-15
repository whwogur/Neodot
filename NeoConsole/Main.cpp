#include <iostream>
#include <Core/src/log/EntryBuilder.h>
#include <Core/src/log/Channel.h>
#include <Core/src/log/MsvcDebugDriver.h>
#include <Core/src/log/TextFormatter.h>
#include <Core/src/log/SeverityLevelPolicy.h>
#include <Core/src/log/Log.h>

using namespace std::string_literals;

std::unique_ptr<Neodot::Log::IChannel> pChan;

void foo()
{
	neolog.error(L"Found an Error!");
}

int main()
{
	Neodot::Log::Init();

	neolog.fatal(L"Fatal Error!");
	neolog.warn(L"Warning");
	foo();

	Neodot::util::StackTrace st1;
	Neodot::util::StackTrace st2 = std::move(st1);

	std::wcout << st2.Print() << std::endl;
	return 0;
}