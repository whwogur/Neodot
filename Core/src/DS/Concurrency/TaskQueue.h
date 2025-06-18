#pragma once
#include <deque> 
#include <future> 
#include <functional> 

namespace Neodot::DS
{
	class Task
	{
	public:
		Task() = default;

		template<typename F>
		requires std::is_invocable_r_v<void, F>
		Task(F&& f): m_callable(std::make_unique<CallableImpl<std::decay_t<F>>>(std::forward<F>(f))) {}

		Task(Task&& other) noexcept = default;
		Task& operator=(Task&& other) noexcept = default;
		Task(const Task&) = delete;
		Task& operator=(const Task&) = delete;

		void operator()() const
		{
			if (!m_callable) throw std::bad_function_call{};
			m_callable->invoke();
		}

	private:
		struct CallableBase {
			virtual ~CallableBase() = default;
			virtual void invoke() const = 0;
		};

		template<typename F>
		struct CallableImpl : CallableBase
		{
			CallableImpl(F&& f) 
				: 
				m_func(std::forward<F>(f)) {}
			
			void invoke() const override { m_func(); }
			F m_func;
		};

		std::unique_ptr<CallableBase> m_callable;
	};

	class TaskQueue
	{
	public:
		template<class F>
		auto Push(F&& function)
		{
			using T = std::invoke_result_t<F>;
			std::packaged_task<T()> pkg{ std::forward<F>(function) };
			auto future = pkg.get_future();
			PushWrappedTask([pkg = std::move(pkg)]() mutable { pkg(); });
			return future;
		}
		void PopExecute();
	private:
		//============
		// functions
		//============
		void PushWrappedTask(Task task);

		//============
		// data
		//============
		std::mutex m_mtx;
		std::deque<Task> m_tasks;
	};
}