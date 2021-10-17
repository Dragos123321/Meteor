#include "meteor/core/thread_pool.h"

namespace mtr
{
	ThreadPool::ThreadPool(size_t num_threads)
	{
		size_t worker_count = 0;

		if (num_threads == USE_HARDWARE_CONCURENCY)
		{
			worker_count = std::thread::hardware_concurrency();
		}		
		else
		{
			worker_count = num_threads;
		}			

		m_threads.reserve(worker_count);

		for (size_t i = 0; i != worker_count; ++i)
		{
			m_threads.emplace_back([this, i]() {
				std::function<void()> task;

				for (;;)
				{
					{
						std::unique_lock lock(m_mutex);

						if (m_tasks.empty())
						{
							m_cond_variable.wait(lock, [this]() {
								return !m_running || !m_tasks.empty();
							});

							if (!m_running) break;
						}

						task = std::move(m_tasks.front());
						m_tasks.pop();
					}

					task();
					task = nullptr;
				}
			});
		}
	}

	ThreadPool::~ThreadPool()
	{
		{
			std::lock_guard lock(m_mutex);
			m_running = false;
		}

		m_cond_variable.notify_all();

		for (auto& thread : m_threads)
		{
			thread.join();
		}
	}

	size_t ThreadPool::worker_count() const
	{
		return m_threads.size();
	}
}