#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

std::vector<int> vec;
void push()
{
	for (int i = 0; i != 10; ++i)
	{
		std::cout << "Push " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		vec.push_back(i);
	}
}
void pop()
{
	for (int i = 0; i != 10; ++i)
	{
		if (vec.size() > 0)
		{
			int val = vec.back();
			vec.pop_back();
			std::cout << "Pop " << val << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

int main()
{
	//create two threads
	std::thread push(push);
	std::thread pop(pop);
	if (push.joinable())
		push.join();
	if (pop.joinable())
		pop.join();

	return 0;
}