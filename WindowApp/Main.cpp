#include <Core/src/window/IWindow.h>
#include <Core/src/Log/Log.h>
#include <Core/src/IOC/Container.h>
#include <Core/src/Log/SeverityLevelPolicy.h>
#include <Core/src/window/Init.h>
#include "App.h"

using namespace Neodot;

void Init()
{
	Log::Init();
	IOC::Get().Register<Log::ISeverityLevelPolicy>(
		[] { return std::make_shared<Log::SeverityLevelPolicy>(Log::Level::Info);
	});

	window::Init();
}

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PWSTR pCmdLine,
	_In_ int nCmdShow)
{
	try
	{
		Init();
		std::shared_ptr<window::IWindow> pWindow = IOC::Get().Resolve<window::IWindow>();
		return App::Update(*pWindow);
	}
	catch (const std::exception& e)
	{
		neolog.error(util::ToWide(e.what())).no_line().no_trace();
		MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR | MB_SETFOREGROUND);
	}
	return -1;
}