#include <iostream>
#include <thread>

#if _MSC_VER <= 1800
// VS 2013 doesn't support thread_local keyword
#define thread_local __declspec(thread)
#endif

static thread_local unsigned int thread_var = 0;
static unsigned int global_var = 0;

void foo_thread()
{
	for (int i = 0; i < 10; i++)
	{
		std::cout << std::this_thread::get_id() << ":" << thread_var++ << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}


void foo_global()
{
	for (int i = 0; i < 10; i++)
	{
		std::cout << std::this_thread::get_id() << ":" << global_var++ << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}


int main()
{

	std::thread(foo_global).detach();
	std::thread(foo_global).detach();
	std::thread(foo_global).detach();
	std::thread(foo_global).detach();
	std::thread(foo_global).join();
	std::cout << "GLOBAL = " << global_var << std::endl;
	std::cout << "----------------------------" << std::endl;


	std::thread(foo_thread).detach();
	std::thread(foo_thread).detach();
	std::thread(foo_thread).detach();
	std::thread(foo_thread).detach();
	std::thread(foo_thread).join();
	std::cout << "THREAD = " << thread_var << std::endl;

	return 0;
}