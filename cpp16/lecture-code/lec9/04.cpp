#include <future>
#include <chrono>
#include <iostream>
#include <queue>

// catch exception from thread


int getting_data()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));

	throw 42;
	return 42;
}

void tick()
{
	int i = 10;
	while (i)
	{
		std::cout << ".";
		i--;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int main()
{
	std::thread t1(tick);	// tick 1s
	std::future<int> f = std::async(std::launch::async, getting_data);

	try
	{
		auto i = f.get(); // getting result
		std::cout << i << std::endl;
	}
	catch (int i)
	{
		std::cout << "catch " << i << std::endl;
	}

	t1.join();
	return 0;
}