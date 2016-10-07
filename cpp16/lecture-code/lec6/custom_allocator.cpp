#include <iostream>
#include "DebugAllocator.h"
#include "TestSpeed.h"

struct TFoo
{
	TFoo()
	{
		std::cout << "TFoo::TFoo" << std::endl;
	}
	
	TFoo(const TFoo&)
	{
		std::cout << "TFoo::copy TFoo" << std::endl;
	}
	~TFoo()
	{
		std::cout << "TFoo::~TFoo" << std::endl;
	}
	double asd = 1.1;
};


struct MyStruct
{
	int a = 0;
	double qrw = 1234;
	std::string adfa = "adsfasd";
};
#include <memory>


int main()
{

	{
		std::vector<TFoo, DebugAllocator<TFoo>> intVector;
		intVector.push_back(TFoo());
		intVector.push_back(TFoo());
	}
	return 0;


	{
		TestSpeed<int>::RunTests();
		TestSpeed<std::string>::RunTests();
		TestSpeed<MyStruct>::RunTests();
	}

	return 0;
}