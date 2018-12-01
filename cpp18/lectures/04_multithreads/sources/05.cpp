#include <iostream>
#include <thread>

int main()
{
	std::thread t([](){
		std::cout << std::this_thread::get_id() << std::endl;
	});
	
	t.detach();
	try
	{
		t.join();
	}
	catch (...)
	{
		std::cout << "opan'ki" << std::endl;
	}

	std::thread t2([](){
		std::cout << std::this_thread::get_id() << std::endl;
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	t2.detach();
	if (t2.joinable())
		t.join();
	
	return 0;
}