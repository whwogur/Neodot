#pragma once
#include <exception> 
#include <string> 

namespace Neodot::util
{
	class IException : public std::exception {};

	class BufferedException : public IException
	{
	public:
		BufferedException() = default;
		BufferedException(std::string msg);
		BufferedException(const std::wstring& msg);
		const char* what() const override;

	private:
		std::string m_message;
		mutable std::string m_buffer;
	};
}

#define ND_EX_DEF_FROM(NewType, BaseType) class NewType : public BaseType {using Base = BaseType; public: using Base::Base;} 
#define ND_EX_DEF(NewType) ND_EX_DEF_FROM(NewType, Neodot::util::BufferedException)