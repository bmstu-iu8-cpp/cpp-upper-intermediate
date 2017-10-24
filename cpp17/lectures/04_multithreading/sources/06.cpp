#include <iostream>
#include <thread>

void thread_function()
{
	std::cout << "thread function\n";
}

int main()
{
	std::thread t(&thread_function);

	// std::thread t2 = t; // Copying a thread won't compile:
	std::thread t2 = std::move(t); // from now t.joinable() returns false

	std::cout << std::boolalpha << t.joinable() << std::endl;
	std::cout << std::boolalpha << t2.joinable() << std::endl;

	t2.join();

	std::cout << std::boolalpha << t.joinable() << std::endl;
	std::cout << std::boolalpha << t2.joinable() << std::endl;


	std::cout << std::endl << std::endl << "creating 2 threads" << std::endl;
	t = std::thread(&thread_function);
	t2 = std::thread(&thread_function);

	std::cout << std::boolalpha << t.joinable() << std::endl;
	std::cout << std::boolalpha << t2.joinable() << std::endl;
	t2 = std::move(t); // calling std::terminate()

	return 0;
}