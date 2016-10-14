#include <list>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>
#include <exception>


/*
	Goal is to describe a main idea
	This is a very bad, non-tested and trimmed analogue of gcpp.
	for more details, see https://github.com/hsutter/gcpp
*/

struct invalid_gc
	: public std::exception
{
	invalid_gc() = default;
	const char * what() const override
	{
		return "invalid_gc";
	}
};

class gc_engine;

class i_gc_ptr
{
public:
	friend gc_engine;
	i_gc_ptr() = default;
	virtual ~i_gc_ptr() = default;

protected:
	virtual void * get_ptr() = 0;
};

template<class T>
class gc_ptr :
	public i_gc_ptr
{
	gc_ptr(gc_engine * const h, const T& rhs) :_heap(h)
	{
		_ptr = _heap->construct(rhs);
		_heap->add_obj(this);
	}

	template<class ... Args>
	gc_ptr(gc_engine * const h, Args&& ... a) :_heap(h)
	{
		_ptr = _heap->construct<T>(std::forward<Args>(a)...);
		_heap->add_obj(this);
	}


public:
	friend gc_engine;
	
	gc_ptr(const gc_ptr<T>& rhs) :_heap(rhs._heap)
	{
		_ptr = rhs._ptr;
		_heap->add_obj(this);
	}

	gc_ptr<T>& operator=(const gc_ptr<T>& rhs)
	{
		if (_heap != rhs._heap)
			throw invalid_gc();
		
		this->_ptr = rhs._ptr;
	}

	gc_ptr(gc_engine * const h) :_heap(h)
	{
		_ptr = nullptr;
		h->add_obj(this);
	}

	~gc_ptr()
	{
		_heap->remove_obj(this);
	}

	T * operator->()
	{
		return _ptr;
	}

	gc_ptr<T> & operator =(std::nullptr_t)
	{
		_ptr = nullptr;
		return *this;
	}

	gc_ptr<T> & operator =(const T &v)
	{
		auto p = _heap->make_gc_ptr<T>(v);
		*this = p;
		return *this;
	}

private:
	void * get_ptr() override
	{
		return _ptr;
	}
	T * _ptr;
	gc_engine * _heap;
};


class gc_engine
{
public:
	template<class T>
	friend class gc_ptr;

	template<class T, class ... Args>
	gc_ptr<T> make_gc_ptr(Args&& ... a)
	{
		gc_ptr<T> g = gc_ptr<T>(this, std::forward<Args>(a)...);

		_allocated_memory[g.get_ptr()] = sizeof(T);
		_reachable_memory[g.get_ptr()] = false;
		_dcors[g.get_ptr()] = [](const void* x){static_cast<const T*>(x)->~T(); };
		return g;
	}

	void collect()
	{
		for (auto & o : _reachable_memory)
		{
			o.second = false;
		}

		for (auto & o : _objs)
		{
			if (o->get_ptr())
				_reachable_memory[o->get_ptr()] = true;
		}

		for (auto & m : _reachable_memory)
		{
			if (!m.second)
			{
				destruct(m.first);
				deallocate(m.first, _allocated_memory[m.first]);
				_allocated_memory.erase(m.first);
				_dcors.erase(m.first);
			}
		}

		for (auto i = _reachable_memory.begin(); i != _reachable_memory.end(); )
		{
			auto b = _allocated_memory.find(i->first) == _allocated_memory.end();

			if (b)
				i = _reachable_memory.erase(i);
			else
				++i;
		}

		return;
	}

	~gc_engine()
	{
		collect();
	}

private:
	void remove_obj(i_gc_ptr * const o)
	{
		auto f = std::find(_objs.begin(), _objs.end(), o);
		if (f != _objs.end())
			_objs.erase(f);
	}

	void add_obj(i_gc_ptr * const o)
	{
		_objs.insert(o);
	}

	void deallocate(void * p, size_t t)
	{
		std::cout << "free = " << p << std::endl;
		delete[] p;
	}

	void * allocate(size_t sz)
	{
		auto p = new char[sz];
		std::cout << (void*)p << std::endl;
		return p;
	}

	template<class T>
	T * construct(const T & v)
	{
		return new(allocate(sizeof(T))) T(v);
	}

	void destruct(void * p)
	{
		_dcors[p](p);
	}

private:
	std::set<i_gc_ptr * > _objs;
	std::map<void *, size_t> _allocated_memory;
	std::map<void *, bool> _reachable_memory;
	std::map<void *, void(*)(const void*)> _dcors;
};


struct TFoo {
	int i = 0;
	int i1 = 12340;
	TFoo() : i(0) { }
	TFoo(const TFoo& r) : i(r.i), i1(r.i1)	{ /*std::cout << "copy" << i << std::endl;*/ }
	TFoo(int a) : i(a)	{ }
	~TFoo()	{	/*std::cout << "~TFoo = " << i << std::endl;*/	}
	int I() { return i;	}
};


int main()
{
	gc_engine h;
	
	TFoo tfoo(111);
	{
		auto ptr = h.make_gc_ptr<TFoo>(tfoo);
		ptr = TFoo(123);
	}
	h.collect();

	{
		gc_ptr<std::string> pStr(&h);

		pStr = h.make_gc_ptr<std::string>("asdf");
	}
	h.collect();

	auto p = h.make_gc_ptr<TFoo>(tfoo);
	p = h.make_gc_ptr<TFoo>(tfoo);
	std::cout << "collect" << std::endl;
	h.collect();

	p = h.make_gc_ptr<TFoo>(tfoo);
	std::cout << "collect" << std::endl;
	h.collect();
	{
		auto p2 = h.make_gc_ptr<TFoo>(4);
		h.make_gc_ptr<TFoo>(4);
		h.make_gc_ptr<TFoo>(4);
		h.make_gc_ptr<TFoo>(4);
	}
	std::cout << "collect" << std::endl;
	h.collect();

	auto p3 = h.make_gc_ptr<std::string>("asdfasdfasd");
	p3 = h.make_gc_ptr<std::string>("asdfasdfasd1234");
	p3 = h.make_gc_ptr<std::string>("asdfasdfasd111111111111");
	p3 = h.make_gc_ptr<std::string>("asdfasdfasd23421344423423423");
	p3 = h.make_gc_ptr<std::string>("asdfasdfasd5555555555555555555555555");
	

	p = nullptr;
	h.collect();
	auto p4 = h.make_gc_ptr<std::string>("asdfasdfasd");
	p3 = p4;
	h.collect();

	return 0;
}