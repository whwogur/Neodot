#pragma once
#include "Driver.h"
#include <memory>

namespace Neodot::Log
{
	class MsvcDebugDriver : public ITextDriver
	{
	public:
		MsvcDebugDriver(std::unique_ptr<ITextFormatter> pFormatter = {});
		void Submit(const Entry&) override;
		void SetFormatter(std::unique_ptr<ITextFormatter> pFormatter) override;
	private:
		std::unique_ptr<ITextFormatter> m_pFormatter;
	};
}