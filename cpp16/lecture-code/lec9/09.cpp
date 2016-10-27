#include <future>
#include <chrono>
#include <iostream>
#include <queue>
#include <algorithm>
#include <numeric>
#include <vector>
#include <list>
#include <random>

// parallel sort

const size_t size_to_use_standart_sort = 2000000;

template<class T>
std::vector<T> p_sort(std::vector<T> input)
{
	if (input.size() < size_to_use_standart_sort)
		// I think that
		// in this case p_sort slower than sort
		// so use sort
	{
		std::stable_sort(input.begin(), input.end());
		return input;
	}

	auto divine_it = input.begin() + (input.size() / 2);

	// launch new task to sort first part
	auto future_lower = std::async(p_sort<T>, 
								   std::vector<T>(input.begin(), divine_it));

	// processing second part
	input.erase(input.begin(), divine_it);
	std::vector<T> higher = p_sort<T>(std::move(input));

	auto lower = future_lower.get();

	std::vector<T> result;
	std::merge(higher.begin(), higher.end(),
			   lower.begin(), lower.end(),
			   std::back_inserter(result));

	return result;
}

int main()
{
	// generate data
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> dis(0, size_to_use_standart_sort);

	const size_t size_data = size_to_use_standart_sort * 16;
	std::vector<int> data(size_data);
	std::generate(data.begin(), data.end(),
				  [&mt, &dis]()->int {return dis(mt);} );


	// using parallel sort
	auto start_p_sort = std::chrono::high_resolution_clock::now();
	auto srt = p_sort(data);
	auto p_sort_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_p_sort).count();
	std::cout << p_sort_time << " ms" << std::endl;


	// using standart sort
	auto start_sort = std::chrono::high_resolution_clock::now();
	std::stable_sort(data.begin(), data.end());
	auto sort_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_sort).count();
	std::cout << sort_time << " ms" << std::endl;


	// checking results
	if (!std::equal(data.begin(), data.end(), srt.begin()))
	{
		std::cout << "wtf?" << std::endl;
	}
	else
	{
		// calculating profit
		std::cout << 100. * (sort_time - p_sort_time) / sort_time << "%" << std::endl;
	}
	return 0;
}