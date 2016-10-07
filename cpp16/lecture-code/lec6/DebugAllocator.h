#pragma once
#include <vector>
#include <iostream>

template<class T>
struct DebugAllocator
{
	typedef std::size_t    size_type;
	typedef std::ptrdiff_t difference_type;
	typedef T*             pointer;
	typedef const T*       const_pointer;
	typedef T&             reference;
	typedef const T&       const_reference;
	typedef T              value_type;

	DebugAllocator() = default;

	template<class U>
	DebugAllocator(const DebugAllocator<U>&) {
	 // from rebind
	
	}

	~DebugAllocator() = default;

	T * allocate(std::size_t num)
	{
		std::cout << "DebugAllocator::alocate " << num << std::endl;
		return static_cast<T*>(::operator new(num * sizeof(T)));
	}

	void deallocate(T * p, std::size_t num)
	{
		std::cout << "DebugAllocator::dealocate " << num << std::endl;
		::operator delete(p);
	}

	// return address of values
	T* address(T& value) const
	{
		return &value;
	}
	const T* address(const T& value) const
	{
		return &value;
	}

	// return maximum number of elements that can be allocated
	std::size_t max_size() const throw()
	{
		return std::numeric_limits<std::size_t>::max() / sizeof(T);
	}

	// initialize elements of allocated storage p with value value
	void construct(T* p, const T& value)
	{
		std::cout << "DebugAllocator::construct " << p << std::endl;
		// initialize memory with placement new
		::new ((void*)p) T(value);
	}

	// destroy elements of initialized storage p
	void destroy(T* p)
	{
		std::cout << "DebugAllocator::destroy " << p << std::endl;
		// destroy objects by calling their destructor
		p->~T();
	}

	// rebind allocator to type U
	template <typename U>
	struct rebind
	{
		typedef DebugAllocator<U> other;
	};
};

template <typename T1, typename T2>
bool operator== (const DebugAllocator<T1>&,
				 const DebugAllocator<T2>&)
{
	return true;
}
template <typename T1, typename T2>
bool operator != (const DebugAllocator<T1>&,
				 const DebugAllocator<T2>&)
{
	return false;
}
