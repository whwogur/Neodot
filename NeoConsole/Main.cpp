#include <iostream>
#include <Core/src/ioc/Container.h>
#include <Core/src/log/SeverityLevelPolicy.h>
#include <Core/src/log/Log.h>
#include <Core/src/util/Assert.h>

using namespace std::string_literals;

std::unique_ptr<Neodot::Log::IChannel> pChan;

void foo()
{
	neolog.error(L"Found an Error!");
}

void Init()
{
	Neodot::Log::Init();
	Neodot::IOC::Get().Register<Neodot::Log::ISeverityLevelPolicy>(
		[] { return std::make_shared<Neodot::Log::SeverityLevelPolicy>(Neodot::Log::Level::Warn);
		});
}

int main()
{
	Init();

	int x = 0, y = 1;
	//neo_assert(x > y).msg(L"TEST").neo_watch(x).neo_watch(y);
	//neo_check(x > y);

	try
	{
		neo_check(x > y).neo_watch(x).ex();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}