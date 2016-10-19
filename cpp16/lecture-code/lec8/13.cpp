#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>
#include <boost\thread.hpp>


class Foo
{
	boost::shared_mutex _m;
	std::vector<int> _v;

public:
	int read_size()
	{
		// many threads can read data, so use shared_lock
		boost::shared_lock<boost::shared_mutex> lk(_m);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		return _v.size();
	}

	void add_value()
	{
		// only one thread can write data, so use lock_guard
		std::lock_guard<boost::shared_mutex> lk(_m);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		_v.push_back(42);
	}
};

void read(Foo &f)
{
	for (int i = 0; i != 100; ++i)
	{
		std::cout << f.read_size() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void write(Foo &f)
{
	for (int i = 0; i != 10; ++i)
	{
		std::cout << "added" << std::endl;
		f.add_value();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main()
{
	Foo f;

	std::thread(read, std::ref(f)).detach();
	std::thread(read, std::ref(f)).detach();
	std::thread(read, std::ref(f)).detach();
	std::thread(read, std::ref(f)).detach();
	std::thread(write, std::ref(f)).join();

	return 0;
}