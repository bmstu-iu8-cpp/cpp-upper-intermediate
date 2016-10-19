#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

void thread_function()
{
	std::cout << std::this_thread::get_id() << std::endl;
}

int main()
{
	std::vector<std::thread> v;

	for (int u = 0; u < 20; u++)
	{
		v.push_back(std::thread(thread_function));
	}
	std::for_each(v.begin(), v.end(), std::mem_fn(&std::thread::join));

	return 0;
}