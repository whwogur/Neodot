#include "GraphicsError.h"
#include <Core/src/window/Utilities.h>
#include <Core/src/util/String.h>
#include <ranges>
#include <format>

namespace rng = std::ranges;
namespace vw = rng::views;

namespace Neodot::App
{
	CheckerToken checkToken;

	HrPipe::HrPipe(unsigned int hr, std::source_location location) noexcept
		: m_hr(hr)
		, m_location(location)
	{}

	void operator>>(HrPipe pipe, CheckerToken)
	{
		if (FAILED(pipe.m_hr))
		{
			// 에러 내용 출력
			std::string errorString;
			for (char c : util::ToNarrow(window::GetErrorDescription(pipe.m_hr))
				| vw::transform([](char c) { return '\n' == c ? ' ' : c; })
				| vw::filter([](char c) { return '\r'!= c; }))
			{
				errorString += c;
			}

			throw std::runtime_error{
				std::format("Graphics Error: {}\n   {}({})",
				errorString, pipe.m_location.file_name(), pipe.m_location.line())
			};
		}
	}
}