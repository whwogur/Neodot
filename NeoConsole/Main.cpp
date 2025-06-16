#include <iostream>
#include <Core/src/ioc/Container.h>
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
	Neodot::IOC::Get().Register<Neodot::Log::ISeverityLevelPolicy>(
		[] { return std::make_shared<Neodot::Log::SeverityLevelPolicy>(Neodot::Log::Level::Warn);
	});


	neolog.fatal(L"Fatal Error!");
	neolog.warn(L"Warning");
	foo();

	Neodot::util::StackTrace st1;
	Neodot::util::StackTrace st2 = std::move(st1);

	std::wcout << st2.Print() << std::endl;
	return 0;
}