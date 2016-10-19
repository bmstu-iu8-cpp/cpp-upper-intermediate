#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>

void dead_lock()
{
	std::mutex m_a;
	std::mutex m_b;

	std::lock_guard<std::mutex> lk_a(m_a);
	std::thread t_a([&m_a, &m_b](){
		std::lock_guard<std::mutex> lk_a(m_a);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::lock_guard<std::mutex> lk_b(m_b);
	});

	std::thread t_b([&m_a, &m_b](){
		std::lock(m_a, m_b);
		std::lock_guard<std::mutex> lk_b(m_b);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::lock_guard<std::mutex> lk_a(m_a);
	});

	t_a.join();
	t_b.join();
}

int main()
{
	std::thread t([]() {
		while(true) {
			std::cout << ".";
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	});

	std::thread(dead_lock).join();

	t.detach();

	return 0;
}