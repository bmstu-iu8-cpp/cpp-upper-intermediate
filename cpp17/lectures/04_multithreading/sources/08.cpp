#include <future>
#include <chrono>
#include <iostream>
#include <queue>

// future & async simple sample


int getting_data()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	return 42;
}

int getting_data_quick(int i)
{
	return i * i;
}

void tick()
{
	int i = 10;
	while (i)
	{
		std::cout << ".";
		i--;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

int main()
{
	std::thread t1(tick);	// tick 1s
	std::future<int> f = std::async(std::launch::async, getting_data); // launching a long time operation
	// std::future<int> f = std::async(std::launch::deferred, getting_data); // deferred launching a long time operation
	// std::future<int> f = std::async(std::launch::async, getting_data_quick, 10); // launching a quick time operation

	std::cout << std::boolalpha << f.valid() << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // performs other actions
	
	auto i = f.get(); // getting result
	std::cout << i << std::endl;
	std::cout << std::boolalpha << f.valid() << std::endl;

	t1.join();
	return 0;
}