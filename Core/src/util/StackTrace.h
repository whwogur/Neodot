#pragma once
#include <memory>
#include <string>

namespace backward
{
	class StackTrace;
}

namespace Neodot::util
{
	class StackTrace
	{
	public:
		StackTrace(size_t skip = 0);
		StackTrace(const StackTrace& src);
		StackTrace& operator=(const StackTrace& src);
		~StackTrace();
		std::wstring Print() const;

	private:
		std::unique_ptr<backward::StackTrace> m_pTrace; // pointer to implementation
	};
}