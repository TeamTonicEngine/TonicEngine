#pragma once

#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace Core::Threads
{
	class ThreadPool
	{
	private:
		static const unsigned int s_poolSize_ = 10;

		std::thread workers_[s_poolSize_];
		std::queue<std::function<void()>> tasksQueue_;

		std::mutex queueMtx_;
		std::condition_variable waitCondition_;

		bool bStop_ = false;

	public:
		const bool Init()
		{
			for (int id = 0; id < s_poolSize_; id++) // Launch workers
				workers_[id] = std::thread([this]() { WorkerTask(); });

			return true;
		}

		void Destroy()
		{
			bStop_ = true; // Notify workers they have to stop
			waitCondition_.notify_all();

			for (int id = 0; id < s_poolSize_; id++) // Free the workers
				workers_[id].join();
		}

		template <class T>
		void AddToQueue(T&& _func, const std::string& _name = "")
		{
			std::unique_lock<std::mutex> lock(queueMtx_);
			// Add the task to the queue
			tasksQueue_.emplace(std::forward<T>(_func));

			if (!_name.empty())
				DEBUG_LOG("Task %s added to Queue.", _name.c_str());

			// Notify workers that one new task is available
			waitCondition_.notify_one();
		}

	private:
		void WorkerTask()
		{
			while (true)
			{
				std::function<void()> task;

				std::unique_lock<std::mutex> lock(queueMtx_);
				// Wait until there is a task in the queue or the thread should stop
				waitCondition_.wait(lock, [this] { return bStop_ || !tasksQueue_.empty(); });

				if (bStop_ && tasksQueue_.empty())
					return; // Exit the thread if it's time to stop

				// Get the next task from the queue
				task = std::move(tasksQueue_.front());
				tasksQueue_.pop();

				task();
			}
		}
	};
}