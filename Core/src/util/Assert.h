#pragma once  
#include <string>
#include <sstream>  
#include <optional>  
#include "StackTrace.h"  
#include <format>  
#include "Macro.h"  
#include "Exception.h"

namespace Neodot::util
{
	ND_EX_DEF(FailedAssertion);

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
		[[noreturn]]
		void ex();

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

#define neo_check(expr) bool(expr) ? void(0) : (void)Neodot::util::Assertion{ \
	ND_WSTR(expr)\
	, __FILEW__\
	, __FUNCTIONW__\
	, __LINE__\
	, NDD_ACTIVE ? Neodot::util::Assertion::Consequence::Terminate : Neodot::util::Assertion::Consequence::Log\
}  

#define neo_watch(...) ND_DISPATCH_VA(ND_INTERNAL_AW_, __VA_ARGS__) 
#define ND_INTERNAL_AW_(expr) watch((expr), ND_WSTR(expr)) 
#define ND_INTERNAL_AW_1_(z) ND_INTERNAL_AW_(z) 
#define ND_INTERNAL_AW_2_(z, a) ND_INTERNAL_AW_(z).ND_INTERNAL_AW_(a) 
#define ND_INTERNAL_AW_3_(z, a, b) ND_INTERNAL_AW_(z).ND_INTERNAL_AW_(a).ND_INTERNAL_AW_(b) 
#define ND_INTERNAL_AW_4_(z, a, b, c) ND_INTERNAL_AW_(z).ND_INTERNAL_AW_(a).ND_INTERNAL_AW_(b).ND_INTERNAL_AW_(c) 
#define ND_INTERNAL_AW_5_(z, a, b, c, d) ND_INTERNAL_AW_(z).ND_INTERNAL_AW_(a).ND_INTERNAL_AW_(b).ND_INTERNAL_AW_(c).ND_INTERNAL_AW_(d) 
#define ND_INTERNAL_AW_6_(z, a, b, c, d, e) ND_INTERNAL_AW_(z).ND_INTERNAL_AW_(a).ND_INTERNAL_AW_(b).ND_INTERNAL_AW_(c).ND_INTERNAL_AW_(d).ND_INTERNAL_AW_(e) 
#define ND_INTERNAL_AW_7_(z, a, b, c, d, e, f) ND_INTERNAL_AW_(z).ND_INTERNAL_AW_(a).ND_INTERNAL_AW_(b).ND_INTERNAL_AW_(c).ND_INTERNAL_AW_(d).ND_INTERNAL_AW_(e).ND_INTERNAL_AW_(f) 
#define ND_INTERNAL_AW_8_(z, a, b, c, d, e, f, g) ND_INTERNAL_AW_(z).ND_INTERNAL_AW_(a).ND_INTERNAL_AW_(b).ND_INTERNAL_AW_(c).ND_INTERNAL_AW_(d).ND_INTERNAL_AW_(e).ND_INTERNAL_AW_(f).ND_INTERNAL_AW_(g) 
#define ND_INTERNAL_AW_9_(z, a, b, c, d, e, f, g, h) ND_INTERNAL_AW_(z).ND_INTERNAL_AW_(a).ND_INTERNAL_AW_(b).ND_INTERNAL_AW_(c).ND_INTERNAL_AW_(d).ND_INTERNAL_AW_(e).ND_INTERNAL_AW_(f).ND_INTERNAL_AW_(g).ND_INTERNAL_AW_(h)