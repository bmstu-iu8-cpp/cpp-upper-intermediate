#pragma once
#include <memory>

class IntVector
{
	size_t _size = 0;
	int * _arr = nullptr;

public:
	~IntVector()
	{
		delete[] _arr;
	}

	IntVector() = default;
	
	IntVector(size_t arraySize) :
		_size(arraySize)
	{
		_arr = new int[_size];
		memset(_arr, 0, sizeof(int) * _size);
	}

	

	IntVector& operator=(const IntVector& rhs)	
	{
		if (this == &rhs)
			return *this;


		delete[] _arr;
		_size = rhs._size;
		if (_size != 0) {
			_arr = new int[_size];
			memcpy(_arr, rhs._arr, sizeof(int) * _size);
		}

		return *this;
	}

	IntVector(const IntVector& rhs) :
		_size(rhs._size)
	{
		if (_size != 0)
		{
			_arr = new int[_size];
			memcpy(_arr, rhs._arr, sizeof(int) * _size);
		}
	}
	// 1. implement move semantic
	IntVector(IntVector && rhs)
	{
		_size = rhs._size;
		_arr = rhs._arr;
		rhs._arr = nullptr;
	}

	IntVector & operator=(IntVector && rhs)
	{
		if (this == &rhs)
			return *this;

		delete[] _arr;
		_size = rhs._size;
		_arr = rhs._arr;
		rhs._arr = nullptr;

		return *this;
	}

	size_t size()
	{
		return _size;
	}


	// 2. show initializer_list constructor
	// Так поступать не надо, по причине того,
	// что получаем разное при конструировании объекта
	// фигурными скобками и круглыми
	// IntVector v5{1, 2}; - массив из двух элементов со значениями 1 и 2
	// IntVector v6(1, 2); - массив из одного элемента со значением 2
	IntVector(std::initializer_list<int> &&list)
		:_size(list.size())
	{
		int indx = 0;

		_arr = new int[_size];
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			_arr[indx] = *it;
			indx++;
		}
	}



	IntVector(size_t size, int value)
		:_size(size)
	{
		_arr = new int[_size];

		for (int i = 0; i < _size; i++)
		{
			_arr[i] = value;
		}
	}


	// 3. show and implement factory
	template <class ...Args>
	static IntVector * factory(Args && ... arg)
	{
		return new IntVector(std::forward<Args>(arg)...);
	}

	class StupidIterator
	{
		int * const _data;
		size_t _curr = 0;
	public:
		using difference_type = std::ptrdiff_t; //type of it1-it2
		using value_type = int; // type of container value
		using pointer = int*; // type of container value ptr
		using reference = int&; // type of container value reference
		using size_type = size_t; // type of container size
		using iterator_category = std::forward_iterator_tag; // category of

		StupidIterator(int * const vector, size_t indx) :
			_data(vector), _curr(indx)
		{ }
		StupidIterator & operator++()
		{
			++_curr;
			return *this;
		}
		int * operator->()
		{
			return &_data[_curr];
		}
		int & operator*()
		{
			return _data[_curr];
		}
		bool operator==(const StupidIterator& rhs)
		{
			return rhs._data == _data && rhs._curr == _curr;
		}
		bool operator!=(const StupidIterator& rhs)
		{
			return !(this->operator==(rhs));
		}
	};
	StupidIterator begin()
	{
		return StupidIterator(_arr, 0);
	}
	StupidIterator end()
	{
		return StupidIterator(_arr, _size);
	}
};

