#include <Core/src/window/NeoWin.h>
#include <Core/src/IOC/Container.h> 
#include <Core/src/Log/SeverityLevelPolicy.h> 
#include <Core/src/window/Init.h>
#include <Core/src/Log/Log.h> 
#include <Core/src/window/IWindow.h>
#include <ranges> 
#include <algorithm>
#include <iterator>

using namespace Neodot;
using namespace std::string_literals;
using namespace std::chrono_literals;
namespace rng = std::ranges;
namespace vw = rng::views;

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
	int x = 0;

	// 창 10개 생성/ 스레드 블로킹 테스트
	std::vector<std::shared_ptr<window::IWindow>> windowPtrs;
	auto view = vw::iota(0, 10)
		| vw::transform([](auto i) { return IOC::Get().Resolve<window::IWindow>(); });

	windowPtrs.reserve(10);
	std::ranges::copy(view, std::back_inserter(windowPtrs));

	while (!windowPtrs.empty())
	{
		std::erase_if(windowPtrs, [](auto& p) {return p->IsClosing(); });

		for (auto& p : windowPtrs)
		{
			p->SetTitle(std::format(L"Neodot {}🚀", std::wstring(x, L' ')));
		}
		x = (x + 1) % 20;

		std::this_thread::sleep_for(std::chrono::milliseconds(100ms));
	}

	return 0;
}