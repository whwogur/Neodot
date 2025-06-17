#include <iostream>
#include <Core/src/ioc/Container.h>
#include <Core/src/Log/SeverityLevelPolicy.h>
#include <Core/src/Log/Log.h>
#include <Core/src/util/Assert.h>
#include <Core/src/window/WindowClass.h>

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

	auto pWinClass = std::make_shared<Neodot::window::WindowClass>(L"");

	return 0;
}