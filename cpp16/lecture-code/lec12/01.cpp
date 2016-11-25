//#include <thread>
//#include <atomic>
//#include <iostream>
//#include <exception>
//
//// Lock free stack with leaks
//
//template<class T>
//class LeakLockFreeStack
//{
//	struct Node
//	{
//		Node * next = nullptr;
//		T data;
//
//		Node(const T& v)
//			: next(nullptr)
//			, data(v)
//		{ }
//
//		Node(const T& v, Node * n)
//			: next(n)
//			, data(v)
//		{ }
//	};
//
//	std::atomic<Node *> _head = nullptr;
//
//public:
//	LeakLockFreeStack()
//	{ }
//
//	~LeakLockFreeStack()
//	{ }
//
//	bool try_pop(T & data)
//	{
//		Node * old_head = _head.load();
//		if (old_head == nullptr)
//		{
//			return false;
//		}
//
//		while (old_head && !_head.compare_exchange_strong(old_head, old_head->next))
//		{
//			; // empty loop
//		}
//
//		if (old_head == nullptr)
//			return false;
//		// if copy throws exception, then the value will be lost
//		data = old_head->data;
//		return true;
//	}
//
//	void push(const T & data)
//	{
//		Node * new_head = new Node(data, _head.load());
//
//		while (!_head.compare_exchange_strong(new_head->next, new_head))
//		{
//			; // empty loop
//		}
//	}
//};
//
//
//int main()
//{
//	LeakLockFreeStack<int> stack;
//
//	int i = 0;
//	stack.try_pop(i);
//
//	stack.push(1);
//	stack.push(2);
//
//}
