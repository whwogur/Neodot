#include <Core/src/window/NeoWin.h>
#include <Core/src/IOC/Container.h> 
#include <Core/src/Log/SeverityLevelPolicy.h> 
#include <Core/src/window/Init.h>
#include <Core/src/Log/Log.h> 
#include <Core/src/IOC/Singletons.h>
#include <Core/src/window/Window.h>

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
	Init();

	window::Window window{
		IOC::Sing().Resolve<window::IWindowClass>(),
		L"Neodot",
		{ 1280, 1024 }
	};

	while (!window.IsClosing())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	return 0;
}