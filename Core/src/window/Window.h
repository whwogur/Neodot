#pragma once
#include "IWindow.h" 
#include "WindowClass.h" 
#include <string> 
#include <thread> 
#include <semaphore> 
#include <atomic> 
#include <optional>
#include <Core/src/DS/Spatial/Dimensions.h> 
#include <Core/src/DS/Spatial//Vec2.h>
#include <Core/src/DS/Concurrency/TaskQueue.h> 

namespace Neodot::window
{
	class Window : public IWindow
	{
	public:
		Window(std::shared_ptr<IWindowClass> pWindowClass, std::wstring title,
			DS::DimensionsI clientAreaSize, std::optional<DS::Vec2I> position = {});
		HWND GetHandle() const override;
		bool IsClosing() const override;
		std::future<void> SetTitle(std::wstring title) override;
		~Window() override;

	protected:
		//==========
		// constant
		//==========
		static constexpr UINT CustomTaskMessageId = WM_USER + 0;

		//==========
		// functions
		//==========
		virtual void MessageLoop() noexcept;

		LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
		template<class F>
		auto Dispatch(F&& f)
		{
			auto future = m_tasks.Push(std::forward<F>(f));
			NotifyTaskDispatch();
			return future;
		}
		void NotifyTaskDispatch() const;

		//=======
		// data 
		//=======
		mutable DS::TaskQueue m_tasks;
		std::binary_semaphore m_startSignal{ 0 };
		std::thread m_mainLoopThread;
		HWND m_hWnd = nullptr;
		std::atomic<bool> m_bClosing = false;
	};
}