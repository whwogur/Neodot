#pragma once 
#include "Driver.h" 
#include <memory> 
#include <filesystem> 
#include <fstream> 

namespace Neodot::Log
{
	class IFileDriver : public ITextDriver {};

	class SimpleFileDriver : public IFileDriver
	{
	public:
		SimpleFileDriver(std::filesystem::path path, std::shared_ptr<ITextFormatter> pFormatter = {});
		void Submit(const Entry&) override;
		void SetFormatter(std::shared_ptr<ITextFormatter> pFormatter) override;
	private:
		std::wofstream m_file;
		std::shared_ptr<ITextFormatter> m_pFormatter;
	};
}