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

	protected:
		virtual LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept = 0;
	};
}