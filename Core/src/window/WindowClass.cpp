#include "WindowClass.h"
#include "IWindow.h"
#include "Exception.h" 
#include <Core/src/Log/Log.h>
#include <Core/src/util/Assert.h>

namespace Neodot::window
{
	// ������ �޽����� �ش� IWindow �ν��Ͻ��� �����ϴ� �Լ�
	LRESULT IWindowClass::ForwardMessage(IWindow* pWnd, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// ���� ���� �Լ� HandleMessage ȣ��
		return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
	}

	// �����ڿ��� ������ Ŭ���� ���
	WindowClass::WindowClass(const std::wstring& className)
		: m_hInstance{ GetModuleHandle(nullptr) }
	{
		const WNDCLASSEXW wc
		{
			.cbSize = sizeof(wc),
			.style = CS_OWNDC,
			.lpfnWndProc = &WindowClass::HandleMessageSetup, // ���� �޽��� ó��
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

		// ������ Ŭ������ ����ϰ� atom�� ����
		m_atom = RegisterClassExW(&wc);

		if (!m_atom)
		{
			neolog.error().hr();
			throw WindowException{};
		}
	}

	// ��ϵ� Ŭ������ ATOM �� ��ȯ
	ATOM WindowClass::GetAtom() const
	{
		return m_atom;
	}

	// �ν��Ͻ� �ڵ� ��ȯ
	HINSTANCE WindowClass::GetInstance() const
	{
		return m_hInstance;
	}

	WindowClass::~WindowClass()
	{
		// ������ Ŭ���� ��� ����
		if (!UnregisterClass(MAKEINTATOM(m_atom), m_hInstance))
		{
			neolog.warn().hr();
		}
	}

	// ���� �޽��� ó�� �Լ� ( ������ ���� ���� 1ȸ�� )
	LRESULT WindowClass::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// CreateWindowExW() ���� ������ �Ķ���͸� ����ؼ� WinAPI���� ������ Ŭ���� �����͸� �������ش�
		if (WM_NCCREATE == msg)
		{
			// ���� �Ķ���� CreateWindowExW()���� ����� ���� ������ ����
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			IWindow* const pWnd = static_cast<IWindow*>(pCreate->lpCreateParams);
			neo_check(pWnd);
			// WinAPI�� ����� �����ͷ� ������ �ν��Ͻ� ������ ����
			SetLastError(0);
			SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			if (auto hr = GetLastError())
			{
				neolog.warn().hr(hr);
			}

			// ���� �޽����� ó���� �Ϲ� �ڵ鷯�� �޽��� ��ƾ ����
			SetLastError(0);
			SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowClass::HandleMessageThunk));
			if (auto hr = GetLastError())
			{
				neolog.warn().hr(hr);
			}

			// ���� �޽����� ������ �ν��Ͻ����� ����
			return ForwardMessage(pWnd, hWnd, msg, wParam, lParam);
		}

		// WM_NCCREATE ���� �޽����� �⺻ ó��
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	// �Ϲ� �޽��� ó�� �Լ�
	LRESULT WindowClass::HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// WinAPI�� �����ص� ����� �����Ϳ��� ������ �ν��Ͻ� ������ ����
		IWindow* const pWnd = reinterpret_cast<IWindow*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
		neo_assert(pWnd);

		// �޽����� �ش� �ν��Ͻ��� HandleMessage�� ����
		return ForwardMessage(pWnd, hWnd, msg, wParam, lParam);
	}
}