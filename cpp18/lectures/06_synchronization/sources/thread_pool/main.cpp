#include "thread_pool.h"

#include <iostream>

void foo(int time)
{
	std::cout << "begin on " << std::this_thread::get_id() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(time));
	std::cout << "end on " << std::this_thread::get_id() << std::endl;
}

int bar(int time)
{
	std::cout << "returns " << time << " on " << std::this_thread::get_id() << std::endl;
	return time;
}

int main()
{
	thread_pool pool(8);

	pool.submit(foo, 10);
	pool.submit(foo, 10);
	pool.submit(foo, 10);
	pool.submit(foo, 10);
	auto f = pool.submit([](int time)
	{
		std::cout << "returns " << time << " on " << std::this_thread::get_id() << std::endl;
		return time;
	}, 10);
	pool.submit(bar, 10).get();


	std::cout << f.get();
	std::this_thread::sleep_for(std::chrono::seconds(2));
}