#include <future>
#include <chrono>
#include <iostream>
#include <queue>
#include <algorithm>
#include <numeric>

// std::promise simple sample

void accumulate(std::vector<int>::iterator first,
				std::vector<int>::iterator last,
				std::promise<int>& accumulate_promise)
{
	int sum = std::accumulate(first, last, 0);
	accumulate_promise.set_value(sum);  // Notify future
}

int main()
{
	std::vector<int> data {0, 1, 2, 3, 4, 6, 7, 132, 134};
	std::promise<int> pr;

	auto f = pr.get_future();
	std::thread t1(accumulate, data.begin(), data.end(),
				   std::ref(pr));

	f.wait();

	std::cout << f.get() << std::endl;
	t1.join();
	return 0;
}