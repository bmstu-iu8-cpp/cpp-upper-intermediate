#include <iostream>
#include <thread>


 //1. simple thread
 //2. exception in thread
 //3. callable item
 //4. join or detach
 //5. arguments


//struct callableObject
//{
//	void operator()()
//	{
//		std::cout << this << std::endl;
//	}
//};


int main()
{
	std::cout << std::this_thread::get_id() << std::endl;
	std::thread([](){
		std::cout << std::this_thread::get_id() << std::endl;
	}).join();

	/*	std::thread(callableObject{}).join();
	std::thread(callableObject{}).join();*/
	return 0;
}