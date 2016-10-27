#include <future>
#include <chrono>
#include <iostream>
#include <queue>
#include <algorithm>
#include <numeric>

// throw exception

void source(std::promise<void> & p)
{
	try
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		throw 42;
		p.set_value();
	}
	catch (...)
	{
		p.set_exception(std::current_exception());
	}
}

void destination(std::future<void> & f)
{
	try
	{
		// f.wait(); // doesn't throw exception
		f.get(); // throw exception
		std::cout << "a signal from source is received" << std::endl;
	}
	catch (int i)
	{
		std::cout << "catch " << i << std::endl;
	}
	
}

int main()
{
	std::promise<void> p;

	auto f = p.get_future();
	std::thread t1(source, std::ref(p));
	std::thread t2(destination, std::ref(f));

	t1.join();
	t2.join();
	return 0;
}