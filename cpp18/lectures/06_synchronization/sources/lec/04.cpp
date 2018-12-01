#include <future>
#include <chrono>
#include <iostream>
#include <queue>

// cv simple sample. testing queue

std::mutex m;
std::queue<int> q;
std::condition_variable cv;

void consumer()
{
	int r = -1;
	while (r != 9)
	{
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [](){
			std::cout << "..";
			return !q.empty();
		}); // wait while q.empty()

		// lk is already locked
		// we can using queue
		r = q.front();
		q.pop();
		lk.unlock();

		std::cout << std::endl << std::boolalpha << r;
		std::this_thread::sleep_for(std::chrono::microseconds(100)); // some action
	}	
}

void producer()
{
	int i = 0;
	while (i < 10)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // some action
		std::lock_guard<std::mutex> lk(m);	// lock
		q.push(i++);						// push data
		cv.notify_one();					// data is ready
	}
}

int main()
{
	std::thread t1(consumer);
   std::thread t2(producer);
   std::thread t3(producer);

	t1.join();
   t2.join();
   t3.join();

	return 0;
}