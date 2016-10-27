#include <future>
#include <chrono>
#include <iostream>

// cv simple sample

std::mutex m;
bool flag = false;
std::condition_variable cv;

void waiting_cv()
{
	std::unique_lock<std::mutex> lk(m);

	cv.wait(lk, [](){
		std::cout << "..";
		return flag;
	});

	// using flag
	std::cout << std::endl << std::boolalpha << flag;
}

void process()
{
	std::lock_guard<std::mutex> lk(m);
	std::this_thread::sleep_for(std::chrono::milliseconds(500)); // some action
	flag = true;
	cv.notify_one(); // data is ready
}

int main()
{
	std::thread t1(waiting_cv);
	std::thread t2(process);

	t1.join();
	t2.join();

	return 0;
}