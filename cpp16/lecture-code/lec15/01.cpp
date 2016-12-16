
#include <iostream>
#include <vector>

template <class T>
struct has_iterator
{
	typedef char yes[1];
	typedef char no[2];

	template <typename C>
	static yes& test(typename C::iterator *);

	template <typename C>
	static no& test(...);

	// If the "sizeof" of the result of calling test<T>(nullptr) is equal to sizeof(yes),
	// the first overload worked and T has a nested type named foobar.
	static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};



template <class D, class B>
struct is_base_derived
{
	typedef char yes[1];
	typedef char no[2];

	static yes& test(const B &);

	static no& test(...);

	static D makeD();

	// If the "sizeof" of the result of calling test<T>(nullptr) is equal to sizeof(yes),
	// the first overload worked and T has a nested type named foobar.
	static const bool value = sizeof(test(makeD())) == sizeof(yes);
};



template<bool _Test,
	class _Ty = void>
	struct enable_if
{ };

template<class _Ty>
struct enable_if<true, _Ty>
{
	typedef _Ty type;
};




template<bool _Test,
	class _Ty = void>
	using enable_if_t = typename enable_if<_Test, _Ty>::type;


template <class D, class B>
B * creating(enable_if_t<is_base_derived<D, B>::value> * = nullptr)
{
	return new D { };
}

template <class D, class B>
B * creating(enable_if_t<!is_base_derived<D, B>::value> * = nullptr)
{
	return new B { };
}


struct Base
{
	virtual void p()
	{
		std::cout << "Base" << std::endl;
	}
};

struct fromBase : public Base
{
	void p() override
	{
		std::cout << "fromBase" << std::endl;
	}
};
int main()
{

	auto ptr = creating<fromBase, Base>();
	ptr->p();

	ptr = creating<int, Base>();
	ptr->p();

	std::cout << std::boolalpha;

	std::cout << has_iterator<std::vector<int>>::value << std::endl;
	std::cout << has_iterator<int>::value << std::endl;

	system("pause");
}