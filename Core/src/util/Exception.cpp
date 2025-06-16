#include "Exception.h" 
#include "String.h" 
#include <format> 
#include <typeinfo> 

namespace Neodot::util
{
	BufferedException::BufferedException(std::string msg)
		: m_message{ std::move(msg) }
	{
	}

	BufferedException::BufferedException(const std::wstring& msg)
		: m_message{ ToNarrow(msg) }
	{
	}

	const char* BufferedException::what() const
	{
		using namespace std::string_literals;
		
		m_buffer = "["s + typeid(const_cast<BufferedException&>(*this)).name() + "]"s;
		if (!m_message.empty())
		{
			m_buffer += ": ";
			m_buffer += m_message;
		}

		return m_buffer.c_str();
	}
}