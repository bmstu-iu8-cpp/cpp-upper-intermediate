//#include <thread>
//#include <atomic>
//#include <iostream>
//#include <exception>
//
//
//template<class T>
//class GreedyLockFreeStack
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
//	void _free_list(Node * p)
//	{
//		while (p)
//		{
//			auto t = p->next;
//			delete p;
//			p = t;
//		}
//	}
//
//	std::atomic<Node *> _head = nullptr;
//	std::atomic<Node *> _to_delete = nullptr;
//
//public:
//	GreedyLockFreeStack()
//	{ }
//
//	~GreedyLockFreeStack()
//	{
//		_free_list(_to_delete.load());
//		_free_list(_head.load());
//	}
//
//	bool try_pop(T & data)
//	{
//		Node * old_head = _head.load();
//		if (old_head == nullptr)
//		{
//			return false;
//		}
//
//		while (!_head.compare_exchange_strong(old_head, old_head->next))
//		{
//			; // empty loop
//		}
//
//		if (old_head == nullptr)
//			return false;
//
//		// if copy throws exception, then the value will be lost
//		data = old_head->data;
//
//		// add to list
//		old_head->next = _to_delete.load();
//		while (!_to_delete.compare_exchange_strong(old_head->next, old_head))
//		{
//			; // empty loop
//		}
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
//struct MyStruct
//{
//	~MyStruct()
//	{
//		std::cout << "del" << std::endl;
//	}
//};
//
//int main()
//{
//	{
//		GreedyLockFreeStack<MyStruct> stack;
//
//		int i = 0;
//
//		stack.push(MyStruct { });
//		stack.push(MyStruct { });
//	
//		MyStruct s;
//		stack.try_pop(s);
//	}
//	return 0;
//}
