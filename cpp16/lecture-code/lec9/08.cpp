#include <future>
#include <chrono>
#include <iostream>
#include <queue>
#include <algorithm>
#include <numeric>

// Demonstrate using std::packaged_task

using packaged_task_and_arg = std::pair<std::packaged_task<double(double)>, double>;

void sequence_invoke(std::vector<packaged_task_and_arg>& v)
{
	for (auto & p : v)
	{
		p.first(p.second);
	}
}


void some_stupid_sample()
{
	auto lambda = [](double d) -> double {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		return d * d;
	};

	std::vector<packaged_task_and_arg> arr;

	for (int i = 1; i <= 6; i++)
	{
		arr.push_back({std::packaged_task<double(double)>(lambda), 10. * i});
	}

	std::vector<std::future<double>> futures;

	for (auto & pt : arr)
		futures.push_back(pt.first.get_future());

	std::thread(sequence_invoke, std::ref(arr)).detach();

	std::cout << futures[futures.size() - 1].get() << std::endl;
}

void simple_sample()
{
	std::packaged_task<double(double)> pt(
		[](double d) -> double {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			return d * d;
		});
		
	auto f = pt.get_future();
			
	std::thread(std::move(pt), 10.).detach();

	std::cout << f.get() << std::endl;
}

int main()
{
	simple_sample();

	some_stupid_sample();
	
	return 0;
}