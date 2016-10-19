//
//#include <iostream>
//#include <thread>
//#include <string>
//#include <chrono>
//#include <vector>
//
//const int count_of_element = 100000000;
//using test_type = double;
//
///*
//	auto start = std::chrono::steady_clock::now();
//	f(s); // execute the function
//	return std::chrono::duration_cast<std::chrono::milliseconds>(
//	std::chrono::steady_clock::now() - start).count();
//*/
//
//int main()
//{
//	std::cout << std::thread::hardware_concurrency() << std::endl;
//	{
//	auto start = std::chrono::steady_clock::now();
//	std::vector<test_type> data(count_of_element, test_type { });
//	test_type res{};
//	for (auto i = 0; i < data.size(); i++)
//	{
//		res += data[i];
//	}
//	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
//		std::chrono::steady_clock::now() - start).count() << std::endl;
//	}
//
//	auto start1 = std::chrono::steady_clock::now();
//	//auto l = ;
//
//	std::thread t1([](){
//		std::vector<test_type> data(count_of_element / 4, test_type { });
//		test_type res { };
//
//		for (auto i = 0; i < data.size(); i++)
//		{
//			res += data[i];
//		}
//	});
//	std::thread t2([](){
//		std::vector<test_type> data(count_of_element / 4, test_type { });
//		test_type res { };
//
//		for (auto i = 0; i < data.size(); i++)
//		{
//			res += data[i];
//		}
//	});
//	std::thread t3([](){
//		std::vector<test_type> data(count_of_element / 4, test_type { });
//		test_type res { };
//
//		for (auto i = 0; i < data.size(); i++)
//		{
//			res += data[i];
//		}
//	});
//	std::thread t4([](){
//		std::vector<test_type> data(count_of_element / 4, test_type { });
//		test_type res{};
//
//		for (auto i = 0; i < data.size(); i++)
//		{
//			res += data[i];
//		}
//	});
//
//
//	t1.join();
//	t2.join();
//	t3.join();
//	t4.join();
//	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
//		std::chrono::steady_clock::now() - start1).count() << std::endl;
//
//	system("pause");
//	return 0;
//}