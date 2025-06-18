#include "TaskQueue.h"

namespace Neodot::DS
{
	void TaskQueue::PopExecute()
	{
		if (m_tasks.empty()) return;

		Task task;
		{
			std::lock_guard lck{ m_mtx };
			task = std::move(m_tasks.front());
			m_tasks.pop_front();
		}

		task();
	}

	void TaskQueue::PushWrappedTask(Task task)
	{
		std::lock_guard lck{ m_mtx };
		m_tasks.push_back(std::move(task));
	}
}