#pragma once
#include "Driver.h"
#include <memory>

namespace Neodot::Log
{
	class IMsvcDebugDriver : public ITextDriver {};

	class MsvcDebugDriver : public IMsvcDebugDriver
	{
	public:
		MsvcDebugDriver(std::shared_ptr<ITextFormatter> pFormatter = {});
		void Submit(const Entry&) override;
		void Flush() override;
		void SetFormatter(std::shared_ptr<ITextFormatter> pFormatter) override;

	private:
		std::shared_ptr<ITextFormatter> m_pFormatter;
	};
}