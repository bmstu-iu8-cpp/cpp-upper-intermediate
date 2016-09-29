#include <tuple>
#include <string>
#include <iostream>
#include <vector>

template<class T, class U>
std::pair<T, U> make_pair(T t, U u)
{
	return std::pair<T, U>(t, u);
}

//int main()
//{
//	auto pair = make_pair(10, 10.111);
//	std::pair<int, double> p(10, 10.1111);
//}

#include "any.h"


int main()
{
	any a1 = 0;
	std::cout << any_cast<int>(a1) << std::endl;

	a1 = static_cast<const char*>("const cahar pointer");
	std::cout << any_cast<const char*>(a1) << std::endl;

	a1 = 2.1214214;
	std::cout << any_cast<double>(a1) << std::endl;

	a1 = std::string("some test string");
	std::cout << any_cast<std::string>(a1) << std::endl;

	try
	{
		std::cout << any_cast<int>(a1) << std::endl;
	}
	catch (bad_any_cast & e)
	{
		std::cout << e.what() << std::endl;
	}

	std::vector<int> vec = {1, 2, 3};
	a1 = vec;
	std::cout << any_cast<std::vector<int>>(a1).back() << std::endl;

	std::vector<any> vec2 = {1, std::string("asdf"), 3.13241234, 132541.1234};

	std::for_each(vec2.begin(), vec2.end(),
		[](const any & obj)
		{
			try
			{
				std::cout << any_cast<double>(obj) << std::endl;
			}
			catch (bad_any_cast & e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	);
}