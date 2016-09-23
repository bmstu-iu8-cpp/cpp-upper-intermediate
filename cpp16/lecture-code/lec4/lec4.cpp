#include <tuple>
#include <string>
#include <iostream>

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
	any a;
	any a1(123041);
	any a2 = a1;

	a1 = std::string("asdfasdf");

	std::cout << any_cast<std::string>(a1);

	try
	{
		std::cout << any_cast<int>(a1);
	}
	catch (std::bad_cast&e)
	{
		e.what();
	}
}