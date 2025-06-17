#include "WindowClass.h"
#include "IWindow.h"
#include "Exception.h" 
#include <Core/src/Log/Log.h>
#include <Core/src/util/Assert.h>

namespace Neodot::window
{
	// 윈도우 메시지를 해당 IWindow 인스턴스로 전달하는 함수
	LRESULT IWindowClass::ForwardMessage(IWindow* pWnd, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// 순수 가상 함수 HandleMessage 호출
		return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
	}

	// 생성자에서 윈도우 클래스 등록
	WindowClass::WindowClass(const std::wstring& className)
		: m_hInstance{ GetModuleHandle(nullptr) }
	{
		const WNDCLASSEXW wc
		{
			.cbSize = sizeof(wc),
			.style = CS_OWNDC,
			.lpfnWndProc = &WindowClass::HandleMessageSetup, // 최초 메시지 처리
			.cbClsExtra = 0,
			.cbWndExtra = 0,
			.hInstance = m_hInstance,
			.hIcon = nullptr,
			.hCursor = nullptr,
			.hbrBackground = nullptr,
			.lpszMenuName = nullptr,
			.lpszClassName = className.c_str(),
			.hIconSm = nullptr,
		};

		// 윈도우 클래스를 등록하고 atom에 저장
		m_atom = RegisterClassExW(&wc);

		if (!m_atom)
		{
			neolog.error().hr();
			throw WindowException{};
		}
	}

	// 등록된 클래스의 ATOM 값 반환
	ATOM WindowClass::GetAtom() const
	{
		return m_atom;
	}

	// 인스턴스 핸들 반환
	HINSTANCE WindowClass::GetInstance() const
	{
		return m_hInstance;
	}

	WindowClass::~WindowClass()
	{
		// 윈도우 클래스 등록 해제
		if (!UnregisterClass(MAKEINTATOM(m_atom), m_hInstance))
		{
			neolog.warn().hr();
		}
	}

	// 최초 메시지 처리 함수 ( 윈도우 생성 직후 1회만 )
	LRESULT WindowClass::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// CreateWindowExW() 에서 가져온 파라미터를 사용해서 WinAPI측에 윈도우 클래스 포인터를 저장해준다
		if (WM_NCCREATE == msg)
		{
			// 생성 파라미터 CreateWindowExW()에서 사용자 지정 포인터 추출
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			IWindow* const pWnd = static_cast<IWindow*>(pCreate->lpCreateParams);
			neo_check(pWnd);
			// WinAPI에 사용자 데이터로 윈도우 인스턴스 포인터 저장
			SetLastError(0);
			SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			if (auto hr = GetLastError())
			{
				neolog.warn().hr(hr);
			}

			// 이후 메시지를 처리할 일반 핸들러로 메시지 루틴 변경
			SetLastError(0);
			SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowClass::HandleMessageThunk));
			if (auto hr = GetLastError())
			{
				neolog.warn().hr(hr);
			}

			// 최초 메시지를 윈도우 인스턴스에게 전달
			return ForwardMessage(pWnd, hWnd, msg, wParam, lParam);
		}

		// WM_NCCREATE 이전 메시지는 기본 처리
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	// 일반 메시지 처리 함수
	LRESULT WindowClass::HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// WinAPI에 저장해둔 사용자 데이터에서 윈도우 인스턴스 포인터 추출
		IWindow* const pWnd = reinterpret_cast<IWindow*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
		neo_assert(pWnd);

		// 메시지를 해당 인스턴스의 HandleMessage로 전달
		return ForwardMessage(pWnd, hWnd, msg, wParam, lParam);
	}
}