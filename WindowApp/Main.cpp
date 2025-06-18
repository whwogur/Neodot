#include <Core/src/window/NeoWin.h>
#include <Core/src/IOC/Container.h> 
#include <Core/src/Log/SeverityLevelPolicy.h> 
#include <Core/src/window/Init.h>
#include <Core/src/Log/Log.h> 

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
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR pCmdLine,
	int nCmdShow)
{
	Init();

	neolog.info(L"Neodot Window");
	MessageBoxA(nullptr, "", "Hello World", MB_APPLMODAL | MB_ICONEXCLAMATION);

	return 0;
}