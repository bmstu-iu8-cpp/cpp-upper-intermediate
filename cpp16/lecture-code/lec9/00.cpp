#include <future>
#include <chrono>
#include <iostream>
#include <queue>
#include <algorithm>
#include <numeric>

// Demonstrate data race

unsigned long long g_sum = 0;

void summ(unsigned long long a, unsigned long long b)
{
	for (unsigned long long i = a; i < b; i++)
	{
		g_sum = g_sum + i;
	}
}

int main()
{
	const unsigned long long n = 10000000;

	std::thread t1(summ, 0, n);
	std::thread t2(summ, n, 2 * n);
	std::thread t3(summ, 2 * n, 3 * n);
	std::thread t4(summ, 3 * n, (4 * n) + 1);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	std::cout << "summ = " << g_sum << std::endl;
	std::cout << "answer = " << (n * 4) * (4 * n + 1) / 2 << std::endl;


	return 0;
}