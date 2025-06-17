#include "Utilities.h"
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
}