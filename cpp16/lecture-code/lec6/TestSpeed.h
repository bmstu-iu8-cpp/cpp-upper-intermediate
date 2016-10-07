#pragma once
#include <vector>
#include <chrono>
#include <functional>
#include <boost\pool\pool_alloc.hpp>
#define BOOST_POOL_NO_MT

template<class Type>
class TestSpeed
{
public:
	static void TestDefaultAllocator(int size)
	{
		std::vector<Type,
			std::allocator<Type >> v;
		//srand(101);
		for (int i = 0; i < size; ++i)
		{
			v.push_back(Type{});
		}
	}

	static void TestBoostAllocator(int size)
	{
		
		std::vector<Type,
			boost::pool_allocator
			<
			Type,
				boost::default_user_allocator_new_delete,
				boost::details::pool::null_mutex,
				32,
				1024
			>> v;
		//srand(101);
		for (int i = 0; i < size; ++i)
		{
			v.push_back(Type{});
		}
		
	}

	static void RunTests()
	{
		auto timing = [](std::function<void(int)> f, int s)
		{
			auto start = std::chrono::steady_clock::now();
			f(s); // execute the function
			return std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::steady_clock::now() - start).count();
		};

		std::vector<int> sizes;

		srand(101);

		const int k = 10000;
		sizes.push_back((rand() % 5) * 100);
		sizes.push_back((rand() % 5) * 100);
		sizes.push_back((rand() % 5) * 100);
		sizes.push_back((rand() % 5) * 100);
		sizes.push_back((rand() % 5) * 100);
		sizes.push_back((rand() % 5) * k);
		sizes.push_back((rand() % 5) * k);
		sizes.push_back((rand() % 5) * k);
		sizes.push_back((rand() % 5) * k);
		sizes.push_back((rand() % 5) * k);
		sizes.push_back((rand() % 5) * k);
		sizes.push_back((rand() % 5) * k);
		sizes.push_back((rand() % 5) * k);
		sizes.push_back((rand() % 5) * k);
		sizes.push_back((rand() % 5) * k);

		std::vector<long long> times1;

		for (auto s : sizes)
		{
			times1.push_back(timing(TestDefaultAllocator, s));
		}
		std::cout << "sizes : " << std::endl;

		for (auto t : sizes)
			std::cout << t << " ";
		std::cout << std::endl;

		std::cout << "Default allocator : " << std::endl;
		for (auto t : times1)
			std::cout << t << " ";
		std::cout << std::endl;

		std::vector<long long> times2;
		for (auto s : sizes)
		{
			times2.push_back(timing(TestBoostAllocator, s));
		}

		std::cout << "Boost allocator : " << std::endl;
		for (auto t : times2)
			std::cout << t << " ";
		std::cout << std::endl;
	}

};

