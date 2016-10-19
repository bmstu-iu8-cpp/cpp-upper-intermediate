#include <iostream>
#include <thread>

int main()
{
	std::thread([](){
		for (int i = 0; i < 10; i++)
		{
			std::cout << std::this_thread::get_id() << ":" << i << std::endl;
		}
	}).detach();
	
	std::thread([](){
		for (int i = 0; i < 10; i++)
		{
			std::cout << std::this_thread::get_id() << ":" << i << std::endl;
		}
	}).detach();

	std::thread([](){
		for (int i = 0; i < 10; i++)
		{
			std::cout << std::this_thread::get_id() << ":" << i << std::endl;
		}
	}).detach();

	std::thread([](){
		for (int i = 0; i < 10; i++)
		{
			std::cout << std::this_thread::get_id() << ":" << i << std::endl;
		}
	}).join();
	return 0;
}