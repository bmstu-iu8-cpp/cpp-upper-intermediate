#include <future>
#include <chrono>
#include <iostream>
#include <queue>

// cv simple sample. testing queue

std::mutex m;
std::queue<int> q;
std::condition_variable cv;

void waiting_cv()
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
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); // some action
	}	
}

void waiting_for_cv()
{
	int r = -1;
	while (r != 9)
	{
		std::unique_lock<std::mutex> lk(m);

		// wait for 100 milliseconds
		auto pred_result = cv.wait_for(lk,
					std::chrono::milliseconds(100),
					[](){
						std::cout << "..";
						return !q.empty();
					});

		if (!pred_result) // if q.empty() continue; else we can using queue
			continue;

		// lk is already locked
		// we can using queue
		r = q.front();
		q.pop();
		lk.unlock();

		std::cout << std::endl << std::boolalpha << r;
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); // some action
	}
}

void process()
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
	std::thread t1(waiting_cv);
	std::thread t2(process);

	t1.join();
	t2.join();

	return 0;
}