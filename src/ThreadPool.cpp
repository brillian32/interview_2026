//
// Created by 17305 on 2025/12/15.
//

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

#include "doctest/doctest.h"

class ThreadPool
{
public:
	// 构造函数：启动固定数量的线程
	explicit ThreadPool(const size_t threads) : stop(false)
	{
		for (size_t i = 0; i < threads; ++i)
			workers.emplace_back(
				[this]
				{
					while (true) {
						std::function<void()> task;
						{ // 作用域：锁仅保护队列操作
							std::unique_lock lock(this->queue_mutex);

							// 等待条件：停止 或 队列不为空
							this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });

							// 如果停止且队列为空，线程退出
							if (this->stop && this->tasks.empty())
								return;

							// 取出任务（移动语义优化性能）
							task = std::move(this->tasks.front());
							this->tasks.pop();
						} // 离开作用域自动解锁

						// 执行任务
						task();
					}
				});
	}

	// 析构函数：停止所有线程
	~ThreadPool()
	{
		{
			std::unique_lock lock(queue_mutex);
			stop = true;
		}
		condition.notify_all(); // 唤醒所有等待线程
		for (std::thread &worker : workers)
			worker.join(); // 等待线程结束
	}

	// 提交任务：支持任意函数和参数，返回 future
	template<class F, class... Args>
	//...arg封包，Args 在里面已经被“按类型展开”过了，只是你没看到显式的 Args... 写出来而已。
	auto enqueue(F &&f, Args &&...args) -> std::future<std::invoke_result_t<F, Args...>>
	{
		using return_type = std::invoke_result_t<F, Args...>;

		// 将任务打包为 packaged_task，以便获取返回值
		// 使用 shared_ptr 是为了能够拷贝进 lambda (std::function 要求可拷贝)
		auto task = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...));


		std::future<return_type> res = task->get_future();
		{
			std::unique_lock lock(queue_mutex);

			// 停止后不允许再入队
			if (stop)
				throw std::runtime_error("enqueue on stopped ThreadPool");

			// 将任务封装进 lambda 放入队列
			tasks.emplace([task]() { (*task)(); });
		}
		condition.notify_one(); // 唤醒一个线程去处理
		return res;
	}

private:
	std::vector<std::thread> workers; // 工作线程队列
	std::queue<std::function<void()>> tasks; // 任务队列

	std::mutex queue_mutex; // 互斥锁
	std::condition_variable condition; // 条件变量
	bool stop; // 停止标志
};

void getAustral(std::mutex &mtx)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::unique_lock lock(mtx);
	std::cout << "@4Task executed in thread " << std::this_thread::get_id() << std::endl;
}

TEST_CASE("Thread Pool")
{
	ThreadPool pool(4); // 创建4个线程

	std::mutex mtx;

	// 示例1：提交有返回值的任务
	auto result = pool.enqueue(
		[&](int answer)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::unique_lock lock(mtx);
			std::cout << "@1Task executed in thread " << std::this_thread::get_id() << " " << answer << std::endl;
			return answer;
		},
		11);


	auto result2 = pool.enqueue(
		[&](int answer)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::unique_lock lock(mtx);
			std::cout << "@2Task executed in thread " << std::this_thread::get_id() << " " << answer << std::endl;
			return answer;
		},
		22);


	// 示例2：提交无返回值的任务
	pool.enqueue(
		[&]()
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::unique_lock lock(mtx);
			std::cout << "@3Task executed in thread " << std::this_thread::get_id() << std::endl;
		});
	pool.enqueue(getAustral, std::ref(mtx));

	// 获取结果
	auto res1 = result.get();
	auto res2 = result2.get();
	std::unique_lock lock(mtx);
	std::cout << "Result1: " << res1 << std::endl;
	std::cout << "Result2: " << res2 << std::endl;
	lock.unlock();
}

// 极简版：不支持返回值，不支持传参（需用lambda捕获）
class SimpleThreadPool
{
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;
	std::mutex mtx;
	std::condition_variable cv;
	bool stop = false;

public:
	explicit SimpleThreadPool(size_t n)
	{
		for (size_t i = 0; i < n; ++i) {
			workers.emplace_back(
				[this]
				{
					while (true) {
						std::unique_lock<std::mutex> lock(mtx);
						cv.wait(lock, [this] { return stop || !tasks.empty(); });
						if (stop && tasks.empty())
							return;

						auto task = tasks.front();
						tasks.pop();
						lock.unlock(); // 记得解锁去执行！
						task();
					}
				});
		}
	}

	~SimpleThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(mtx);
			stop = true;
		}
		cv.notify_all();
		for (auto &t : workers)
			t.join();
	}

	// 提交任务很简单
	void enqueue(const std::function<void()> &task)
	{
		{
			std::unique_lock<std::mutex> lock(mtx);
			tasks.push(task);
		}
		cv.notify_one();
	}
};

TEST_CASE("Simple Thread Pool")
{
	SimpleThreadPool pool(4);
	pool.enqueue([]() { std::cout << "@1Task executed in thread " << std::this_thread::get_id() << std::endl; });
	pool.enqueue([]() { std::cout << "@2Task executed in thread " << std::this_thread::get_id() << std::endl; });
}
