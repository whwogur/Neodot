#pragma once  
#include <string>
#include <sstream>  
#include <optional>  
#include "StackTrace.h"  
#include <format>  
#include "Macro.h"  

namespace Neodot::util
{
	class Assertion
	{
	public:
		enum class Consequence
		{
			Log,
			Terminate,
			Exception,
		};

		Assertion(std::wstring expression, const wchar_t* file, const wchar_t* function, int line, Consequence consequence = Consequence::Terminate);
		~Assertion();
		Assertion& msg(const std::wstring& message);

		template<typename T>
		Assertion& watch(T&& val, const wchar_t* name)
		{
			m_stream << L"   " << name << L" => " << std::forward<T>(val) << L"\n";
			return *this;
		}
	private:
		const wchar_t* m_file;
		const wchar_t* m_function;
		int m_line = -1;
		Consequence m_consequence;
		std::wostringstream m_stream;
	};
}

#ifndef NDD_ACTIVE  
#ifdef NDEBUG  
#define NDD_ACTIVE false  
#else  
#define NDD_ACTIVE true  
#endif  
#endif  

#define neo_assert(expr) (!NDD_ACTIVE || bool(expr)) ? void(0) : (void)Neodot::util::Assertion{ ND_WSTR(expr), __FILEW__, __FUNCTIONW__, __LINE__ }  

#define neo_watch(expr) watch((expr), ND_WSTR(expr))

#define neo_check(expr) bool(expr) ? void(0) : (void)Neodot::util::Assertion{ \
	ND_WSTR(expr)\
	, __FILEW__\
	, __FUNCTIONW__\
	, __LINE__\
	, NDD_ACTIVE ? Neodot::util::Assertion::Consequence::Terminate : Neodot::util::Assertion::Consequence::Log \
} 