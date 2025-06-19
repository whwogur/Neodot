#pragma once
#include "NeoWin.h"

namespace Neodot::window
{
	class IWindow
	{
		// Window Ŭ������ private ����� ������ �� �ֵ��� friend ����
		friend class IWindowClass;
	public:
		virtual ~IWindow() = default;
		virtual HWND GetHandle() const = 0;
		virtual bool IsClosing() const = 0;

	protected:
		virtual LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept = 0;
	};
}