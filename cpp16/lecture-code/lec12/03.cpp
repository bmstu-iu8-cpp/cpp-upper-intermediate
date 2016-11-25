//#include <thread>
//#include <atomic>
//#include <iostream>
//#include <exception>
//#include <memory>
//#include <list>
//
//
//std::atomic<int> CondInPop1(0), CondToDelete(0), CondInPop0(0), CondInPopNot0(0), CondInPopNot1(0);
//
//template<class T>
//class RefCountLockFreeStack
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
//	void _free_node(Node * node)
//	{
//		if (_in_pop == 1)
//		{
//			++CondInPop1;
//			Node * to_delete = _to_delete.exchange(nullptr);
//			if (to_delete) 
//			{
//				++CondToDelete;
//				if (_in_pop.fetch_sub(-1) == 0)
//				{
//					++CondInPop0;
//					_free_list(to_delete);
//				}
//				else
//				{
//					++CondInPopNot0;
//					Node *last = to_delete;
//					while (last->next)
//						last = last->next;
//					last->next = _to_delete;
//
//					while (!_to_delete.compare_exchange_weak(last->next, to_delete))
//						;
//				}
//			}
//			else 
//			{
//				_in_pop.fetch_sub(-1);
//			}
//			delete node;
//		}
//		else 
//		{
//			++CondInPopNot1;
//			node->next = _to_delete.load();
//			while (!_to_delete.compare_exchange_weak(node->next, node))
//				;
//			_in_pop.fetch_sub(-1);
//		}
//	}
//
//	std::atomic<Node *> _head = nullptr;
//	std::atomic<int> _in_pop {0};
//	std::atomic<Node *> _to_delete = nullptr;
//
//public:
//	RefCountLockFreeStack()
//	{ }
//
//	~RefCountLockFreeStack()
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
//		// count threads in pop
//		++_in_pop;
//		
//		
//		while (old_head && !_head.compare_exchange_strong(old_head, old_head->next))
//		{
//			; // empty loop
//		}
//
//		if (old_head == nullptr)
//			return false;
//
//		// if copy throws exception, then the value will be lost
//		data = old_head->data;
//		_free_node(old_head);
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
//		RefCountLockFreeStack<MyStruct> stack;
//
//		int i = 0;
//
//		stack.push(MyStruct { });
//		stack.push(MyStruct { });
//
//		MyStruct s;
//		stack.try_pop(s);
//	}
//
//	{
//		const size_t THREADS = 10;
//		const size_t PUSHS = 100000;
//		const size_t READERS = 4;
//
//		std::cout << std::thread::hardware_concurrency() << std::endl;
//		RefCountLockFreeStack<int> s;
//		typedef std::shared_ptr<std::thread> TThreadPtr;
//		std::list<TThreadPtr> threads;
//
//		for (size_t i = 0; i < READERS; ++i) {
//			threads.push_back(TThreadPtr(
//				new std::thread([&]() {
//				//std::this_thread::sleep_for(std::chrono::seconds(1));
//				long long sum = 0;
//				for (size_t i = 0; i < THREADS * PUSHS; ++i) {
//					int t;
//					if (s.try_pop(t)) {
//						sum += t;
//					}
//					else {
//						std::this_thread::yield();
//					}
//				}
//				std::cout << sum << " " << (static_cast<long long>(PUSHS)* (PUSHS - 1) / 2 * THREADS) << std::endl;
//			})));
//		}
//		for (size_t t = 0; t < THREADS; ++t) {
//			threads.push_back(TThreadPtr(new std::thread([&]() {
//				for (int i = 0; i < PUSHS; ++i) {
//					s.push(i);
//				}
//			})));
//		}
//		while (!threads.empty()) {
//			TThreadPtr t = threads.front();
//			threads.pop_front();
//			t->join();
//		}
//		std::cout << CondInPop1 << " " << CondToDelete << " " << CondInPop0 << " "
//			<< CondInPopNot0 << " " << CondInPopNot1 << std::endl;
//
//	}
//
//	system("pause");
//
//	return 0;
//}
