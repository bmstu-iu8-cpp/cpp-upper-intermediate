#include <future>
#include <thread>
#include <queue>
#include <functional>
#include <atomic>


class thread_pool
{
public:

	thread_pool()
	{
		_start();
	}

	thread_pool(size_t num) :
		_num(num)
	{
		_start();
	}

	~thread_pool()
	{
		std::unique_lock<std::mutex> lk(_m);
		_stop = true;
		lk.unlock();

		cv.notify_all();
		for (auto & t : _threads)
			t.join();
	}

	template<class F, class ... Args>
	std::future<std::result_of_t<F(Args...)>> submit(F && func, Args && ... args)
	{
		using result_type = std::result_of_t<F(Args...)>;
		
		auto ptr_task = std::make_shared<std::packaged_task<result_type()>>(
			std::bind(std::forward<F>(func), std::forward<Args>(args)... ));

		auto result = ptr_task->get_future();

		std::lock_guard<std::mutex> lk(_m);
		_tasks.emplace([ptr_task]() -> void
		{
			(*ptr_task)();
		});

		cv.notify_one();
		return result;
	}

private:
	size_t _num = std::thread::hardware_concurrency();
	std::queue<std::function<void(void)>> _tasks;
	std::vector<std::thread> _threads;
	std::mutex _m;
	std::condition_variable cv;
	bool _stop;
	
	void _working_thread_func()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lk(_m);
			cv.wait(lk, [this]()
			{
				return _stop || !this->_tasks.empty(); 
			});

			if (_stop)
				return;

			auto task = _tasks.front();
			_tasks.pop();
			lk.unlock();

			task();
		}
	}

	void _start()
	{
		_stop = false;
		for (size_t i = 0; i < _num; i++)
			_threads.push_back(std::thread(&thread_pool::_working_thread_func, this));
	}
};
