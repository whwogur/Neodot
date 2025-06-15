#pragma once
#include "MsvcDebugDriver.h"
// TODO: replace with custom windows header
#include <Windows.h>
#include "TextFormatter.h"

namespace Neodot::Log
{
	MsvcDebugDriver::MsvcDebugDriver(std::unique_ptr<ITextFormatter> pFormatter)
		: m_pFormatter{ std::move(pFormatter) }
	{
	}
	void MsvcDebugDriver::Submit(const Entry& e)
	{
		if (m_pFormatter) {
			OutputDebugStringW(m_pFormatter->Format(e).c_str());
		}
		// TODO: how to log stuff from log system
	}
	void MsvcDebugDriver::SetFormatter(std::unique_ptr<ITextFormatter> pFormatter)
	{
		m_pFormatter = std::move(pFormatter);
	}
}