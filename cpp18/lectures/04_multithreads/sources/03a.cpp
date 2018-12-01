//#include <future>
//#include <chrono>
//#include <iostream>
//#include <queue>
//
//// future & async simple sample
//
//int getting_data()
//{
//	std::this_thread::sleep_for(std::chrono::seconds(3));
//	return 42;
//}
//
//void print_future(std::shared_future<int> f)
//{
//	std::this_thread::sleep_for(std::chrono::milliseconds(500));
//	std::cout << f.get() << std::endl;
//}
//
//void tick()
//{
//	int i = 4;
//	while (i)
//	{
//		std::cout << ".";
//		i--;
//		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//	}
//}
//
//int main()
//{
//	std::thread t1(tick);	// tick 1s
//	auto sf = std::async(std::launch::async, getting_data).share();
//	
//	std::thread t2(print_future, sf);
//	std::thread t3(print_future, sf);
//	std::thread t4(print_future, sf);
//
//	t1.join();
//	t2.join();
//	t3.join();
//	t4.join();
//	return 0;
//}