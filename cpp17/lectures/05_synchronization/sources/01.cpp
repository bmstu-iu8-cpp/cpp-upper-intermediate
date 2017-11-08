#include <queue>
#include <mutex>
#include <thread>
#include <iostream>

template<class T>
class SafeQueue
{
public:
    void Push(const T& data)
    {
        std::lock_guard<std::mutex> lk(Mutex);
        Queue.push(data);
    }

    T Pop()
    {
        std::lock_guard<std::mutex> lk(Mutex);
        T data = Queue.front();
        Queue.pop();
        return data;
    }

private:
    std::queue<T> Queue;
    std::mutex Mutex;
};

SafeQueue<int> queue;

void foo()
{
    for (int i = 0; i < 10; ++i)
    {
        queue.Push(i);
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}

int main()
{
    std::thread t1(foo);
    std::thread t2(foo);

    t1.join();
    t2.join();

    for (int i = 0; i < 20; ++i)
    {
        std::cout << queue.Pop() << std::endl;
    }

    return 0;
}