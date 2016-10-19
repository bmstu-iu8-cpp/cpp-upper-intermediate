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
		vec.push_back(i);
	}
}

void pop()
{
	for (int i = 0; i != 50; ++i)
	{
		{
			std::lock_guard<std::mutex> lk(m);
			if (vec.size() > 0)
			{
				int val = vec.back();
				vec.pop_back();
				std::cout << "Pop[" << std::this_thread::get_id() << "] " << val << std::endl;
			}
		} // unlock mutex
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
}

void bad_pop()
{
	for (int i = 0; i != 50; ++i)
	{
		if (vec.size() > 0)
		{
			int val = vec.back();
			vec.pop_back();
			std::cout << "Pop[" << std::this_thread::get_id() << "] " << val << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}


int main()
{
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