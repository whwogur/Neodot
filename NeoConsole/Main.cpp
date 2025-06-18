#include <iostream>
#include <Core/src/ioc/Container.h>
#include <Core/src/Log/SeverityLevelPolicy.h>
#include <Core/src/Log/Log.h>
#include <Core/src/util/Assert.h>
#include <Core/src/window/WindowClass.h>

using namespace std::string_literals;

std::unique_ptr<Neodot::Log::IChannel> pChan;

void Init()
{
	Neodot::Log::Init();
	Neodot::IOC::Get().Register<Neodot::Log::ISeverityLevelPolicy>(
		[] { return std::make_shared<Neodot::Log::SeverityLevelPolicy>(Neodot::Log::Level::Info);
		});
}

int main()
{
	Init();

	auto pWinClass = std::make_shared<Neodot::window::WindowClass>();

	neolog.error(L"TEST: no trace").no_trace().no_line();
	neolog.info().trace();

	return 0;
}