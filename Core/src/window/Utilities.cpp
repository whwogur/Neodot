#include "Utilities.h"
#include "Exception.h"
#include <Core/src/Log/Log.h>

namespace Neodot::window
{
	// HRESULT ���� �ڵ�κ��� ����� ���� �� �ִ� �޽��� ���ڿ��� ��ȯ�ϴ� �Լ�
	std::wstring GetErrorDescription(HRESULT hr)
	{
		wchar_t* descriptionWinalloc = nullptr;

		// �ý��ۿ��� ���� �޽����� �������� ���� FormatMessageW ���
		const auto result = FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPWSTR>(&descriptionWinalloc), 0, nullptr
		);

		std::wstring description;
		if (!result)
		{
			// �޽��� ���� ����
			neolog.warn(L"Failed formatting windows error");
		}
		else
		{
			// �޽��� ���� �� WinAPI�� �Ҵ��� �޸� ����
			description = descriptionWinalloc;
			if (LocalFree(descriptionWinalloc))
			{
				neolog.warn(L"Failed freeing memory for windows error formatting");
			}

			// ���ڿ� ���� "\r\n"�� �ٴ� ��� ����
			if (description.ends_with(L"\r\n"))
			{
				description.resize(description.size() - 2);
			}
		}
		return description;
	}

	RECT ToWinRect(const DS::RectI& rect)
	{
		return {
			.left = rect.left,
			.top = rect.top,
			.right = rect.right,
			.bottom = rect.bottom,
		};
	}

	DS::RectI ToRect(const RECT& rWin)
	{
		return {
			.left = rWin.left,
			.top = rWin.top,
			.right = rWin.right,
			.bottom = rWin.bottom,
		};
	}

	DS::DimensionsI ClientToWindowDimensions(const DS::DimensionsI& dims, DWORD styles)
	{
		using namespace DS;
		auto rect = ToWinRect(RectI::FromPointAndDimensions(Vec2I(0, 0), dims));
		if (FALSE == AdjustWindowRect(&rect, styles, FALSE))
		{
			neolog.error(L"Failed to adjust window rect").hr();
			throw WindowException{};
		}
		return ToRect(rect).GetDimensions();
	}
}