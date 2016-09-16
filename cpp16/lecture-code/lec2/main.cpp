#include "IntVector.h"
#include <algorithm>
#include <iostream>

int main()
{
	IntVector v(100);

	IntVector v1 = v; // 
	v = v1;

	IntVector v2 {std::move(v1)};
	//std::cout << v1.size();
	// if (0 < v1.size())
	//	 std::cout << v1[0];

	IntVector v3 = std::move(v);

	v1 = std::move(v3);

	IntVector v4 = {1, 2, 3, 5, 68};
	std::for_each(v4.begin(), v4.end(), [](int i){std::cout << i; });

	IntVector v5 {1, 2};
	IntVector v6(1, 2);

	auto ptr = IntVector::factory(1000);
	auto ptr1 = IntVector::factory(19, 21);
	auto ptr2 = IntVector::factory(v6);
	auto ptr3 = IntVector::factory(IntVector(1, 2));
	// auto l = {1, 2};
	// auto ptr4 = IntVector::factory(l);


	std::cout << std::endl;
	system("pause");
}

