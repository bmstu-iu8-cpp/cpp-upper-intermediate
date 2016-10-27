#include <future>
#include <chrono>
#include <iostream>
#include <queue>
#include <algorithm>
#include <numeric>

// Demonstrate using promise<void> to signal state between threads.

void source(std::promise<void> & p)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	p.set_value();
}

void destination(std::future<void> & f)
{
	f.wait();
	std::cout << "a signal from source is received" << std::endl;
}

int main()
{
	std::promise<void> p;

	auto f = p.get_future();
	std::thread t1(source,
				   std::ref(p)); // use std::move(p) if 'source' takes p by value
	std::thread t2(destination,
				   std::ref(f)); // use std::move(f) if 'destination' takes f by value

	t1.join();
	t2.join();
	return 0;
}