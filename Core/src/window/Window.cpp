#include "Window.h"
#include "Utilities.h"
#include "Exception.h"
#include <format>
#include <Core/src/Log/Log.h>
#include <Core/src/util/String.h>

namespace Neodot::window
{
	Window::Window(std::shared_ptr<IWindowClass> pWindowClass, std::wstring title,
		DS::DimensionsI clientAreaSize, std::optional<DS::Vec2I> position)
		: m_mainLoopThread
		{ 
			&Window::MessageLoop,
			this
		}
	{
		auto future = m_tasks.Push([=, this] {
			const DWORD styles = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
			const DWORD exStyles = 0;
			const auto windowDims = ClientToWindowDimensions(clientAreaSize, styles);
			const auto hModule = GetModuleHandleW(nullptr);
			if (!hModule)
			{
				neolog.error().hr();
				throw WindowException{ "Failed to get module handle" };
			}

			m_hWnd = CreateWindowExW(
				exStyles,
				MAKEINTATOM(pWindowClass->GetAtom()),
				title.c_str(),
				styles,
				position ? position->x : CW_USEDEFAULT,
				position ? position->y : CW_USEDEFAULT,
				windowDims.width, windowDims.height,
				nullptr, nullptr, hModule, this
			);

			if (!m_hWnd)
			{
				neolog.error(L"Failed creating window").hr();
				throw WindowException{ "Failed creating window" };
			}
			});

		m_startSignal.release();
		future.get();
	}

	HWND Window::GetHandle() const
	{
		return m_hWnd;
	}

	bool Window::IsClosing() const
	{
		return m_bClosing;
	}

	std::future<void> Window::SetTitle(std::wstring title)
	{
		return Dispatch([=, this]{
			if (!SetWindowTextW(m_hWnd, title.c_str()))
			{
				neolog.warn().hr();
			}
		});
	}

	Window::~Window()
	{
		Dispatch([this] {
			if (!DestroyWindow(m_hWnd))
			{
				neolog.warn(L"Failed destroying window").hr();
			}
		});

		m_mainLoopThread.join();
	}

	LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		try {
			switch (msg) {
			case WM_DESTROY:
				m_hWnd = nullptr;
				PostQuitMessage(0);
				return 0;
			case WM_CLOSE:
				m_bClosing = true;
				return 0;
			case CustomTaskMessageId:
				m_tasks.PopExecute();
				return 0;
			}
		}
		catch (const std::exception& e)
		{
			neolog.error(std::format(
				L"Uncaught exception in Windows message handler: {}", util::ToWide(e.what())
			));
		}
		catch (...)
		{
			neolog.error(L"Unknown exception in Windows message handler");
		}

		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}

	void Window::NotifyTaskDispatch() const
	{
		if (!PostMessageW(m_hWnd, CustomTaskMessageId, 0, 0))
		{
			neolog.error().hr();
			throw WindowException{ "Failed to post task notification message" };
		}
	}

	void Window::MessageLoop() noexcept
	{
		m_startSignal.acquire();
		m_tasks.PopExecute();

		MSG msg{};
		while (GetMessageW(&msg, m_hWnd, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
}