#include "FileDriver.h"
#include "TextFormatter.h" 

namespace Neodot::Log
{
	SimpleFileDriver::SimpleFileDriver(std::filesystem::path path, std::shared_ptr<ITextFormatter> pFormatter)
		: m_pFormatter{ std::move(pFormatter) }
	{
		// ���°�θ� ����
		std::filesystem::create_directories(path.parent_path());
		// ���� ����, �ִٸ� ��� �̾ ��
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