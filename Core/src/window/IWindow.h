#pragma once
#include "NeoWin.h"
#include <future>
#include <string>
#include <optional>
#include <Core/src/DS/Spatial/Dimensions.h>
#include <Core/src/DS/Spatial/Vec2.h>

namespace Neodot::window
{
	class IWindow
	{
		// Window Ŭ������ private ����� ������ �� �ֵ��� friend ����
		friend class IWindowClass;
	public:
		//=========
		// types
		//=========
		struct IocParams
		{
			std::shared_ptr<IWindowClass>	pClass;
			std::optional<std::wstring>		wName;
			std::optional<DS::DimensionsI>	iSize;
			std::optional<DS::Vec2I>		vPosition;
		};

		virtual ~IWindow() = default;
		virtual HWND GetHandle() const = 0;
		virtual bool IsClosing() const = 0;
		virtual std::future<void> SetTitle(std::wstring title) = 0;

	protected:
		virtual LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept = 0;
	};
}