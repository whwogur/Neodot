#include "FileDriver.h"
#include "TextFormatter.h" 

namespace Neodot::Log
{
	SimpleFileDriver::SimpleFileDriver(std::filesystem::path path, std::shared_ptr<ITextFormatter> pFormatter)
		: m_pFormatter{ std::move(pFormatter) }
	{
		// 없는경로면 만듦
		std::filesystem::create_directories(path.parent_path());
		// 파일 열기, 있다면 열어서 이어서 씀
		m_file.open(path, m_file.out | m_file.app);
	}
	void SimpleFileDriver::Submit(const Entry& e)
	{
		if (m_pFormatter) {
			m_file << m_pFormatter->Format(e).c_str();
		}
	}

	void SimpleFileDriver::Flush()
	{
		m_file.flush();
	}

	void SimpleFileDriver::SetFormatter(std::shared_ptr<ITextFormatter> pFormatter)
	{
		m_pFormatter = std::move(pFormatter);
	}
}