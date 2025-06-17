#include "Utilities.h"
#include <Core/src/Log/Log.h>

namespace Neodot::window
{
	// HRESULT 에러 코드로부터 사람이 읽을 수 있는 메시지 문자열을 반환하는 함수
	std::wstring GetErrorDescription(HRESULT hr)
	{
		wchar_t* descriptionWinalloc = nullptr;

		// 시스템에서 에러 메시지를 가져오기 위해 FormatMessageW 사용
		const auto result = FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPWSTR>(&descriptionWinalloc), 0, nullptr
		);

		std::wstring description;
		if (!result)
		{
			// 메시지 생성 실패
			neolog.warn(L"Failed formatting windows error");
		}
		else
		{
			// 메시지 복사 후 WinAPI가 할당한 메모리 해제
			description = descriptionWinalloc;
			if (LocalFree(descriptionWinalloc))
			{
				neolog.warn(L"Failed freeing memory for windows error formatting");
			}

			// 문자열 끝에 "\r\n"이 붙는 경우 제거
			if (description.ends_with(L"\r\n"))
			{
				description.resize(description.size() - 2);
			}
		}
		return description;
	}
}