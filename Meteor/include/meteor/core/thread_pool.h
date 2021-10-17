#pragma once

#include "meteor/core/non_copyable.h"

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace mtr
{
	class ThreadPool : NonCopyable, NonMovable
	{
	public:
		static const size_t USE_HARDWARE_CONCURENCY = 0;

		explicit ThreadPool(size_t num_threads = USE_HARDWARE_CONCURENCY);
		~ThreadPool();

		template<typename F, typename... Args>
		auto execute(F&& task, Args&& ...args) -> std::future<decltype(task(args...))>;

		size_t worker_count() const;

	private:
		bool m_running = true;
		std::vector<std::thread> m_threads;
		std::queue<std::function<void()>> m_tasks;
		std::mutex m_mutex;
		std::condition_variable m_cond_variable;
	};

#pragma region Implementation
	template<typename F, typename... Args>
	auto ThreadPool::execute(F&& task, Args&& ...args) -> std::future<decltype(task(args...))>
	{
		auto packaged_task = std::make_shared<std::packaged_task<decltype(task(args...))()>>(
			std::bind(std::forward<F>(task), std::forward<Args>(args)...)
		);

		auto future = packaged_task->get_future();

		{
			std::scoped_lock lock(m_mutex);

			m_tasks.emplace([packaged_task]() {
				(*packaged_task)();
			});
		}

		m_cond_variable.notify_one();
		return future;
	}
#pragma endregion
}