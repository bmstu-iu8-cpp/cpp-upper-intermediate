#include <iostream>
#include <thread>

void foo(std::string & s)
{
	s += s;
}

int main()
{
	std::string s = "foo";
	std::thread(foo, s).join();
	std::cout << s.c_str() << std::endl;

	std::thread(foo, std::ref(s)).join();
	std::cout << s.c_str() << std::endl;	
	return 0;
}