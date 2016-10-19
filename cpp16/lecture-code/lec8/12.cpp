#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>

std::mutex m;
std::vector<int> vec;

void push()
{
	for (int i = 0; i != 100; ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::lock_guard<std::mutex> lk(m);
		vec.push_back(i);
	}
}

void pop()
{
	for (int i = 0; i != 50; ++i)
	{
		std::unique_lock<std::mutex> lk(m);
		if (vec.size() > 0)
		{
			int val = vec.back();
			vec.pop_back();
			std::cout << "Pop[" << std::this_thread::get_id() << "] " << val << std::endl;
		}
		lk.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
}

std::unique_lock<std::mutex> pass_own(std::unique_lock<std::mutex> lx)
{
	if (lx)
	{
		std::cout << "unlocking" << std::endl;
		lx.unlock();
	}
	return lx;
}


int main()
{
	// just testing unique_lock
	std::mutex local_mutex;
	std::unique_lock<std::mutex> lx(local_mutex, std::defer_lock);
	lx = pass_own(std::move(lx));
	lx.lock();
	lx = pass_own(std::move(lx));
	auto successed = lx.try_lock();
	std::cout << std::boolalpha << successed << std::endl;
	std::cout << std::boolalpha << pass_own(std::move(lx)).owns_lock() << std::endl;
	

	//create two threads
	std::thread push(push);
	std::thread thread_pop_a(pop);
	std::thread thread_pop_b(pop);
	if (push.joinable())
		push.join();
	if (thread_pop_a.joinable())
		thread_pop_a.join();
	if (thread_pop_b.joinable())
		thread_pop_b.join();


	return 0;
}