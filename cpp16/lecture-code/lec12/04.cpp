//
//#ifdef NDEBUG
//#   define _SECURE_SCL 0
//#endif
//
//#include "./relacy/relacy\pch.hpp"
//
//#include "./relacy/relacy\atomic.hpp"
//#include "./relacy/relacy\relacy_std.hpp"
//
//
//
//class stack
//{
//public:
//	stack()
//		: _head(0)
//	{ }
//
//	~stack()
//	{
//		_free_list(_to_delete.load(rl::memory_order_seq_cst));
//		_free_list(_head.load(rl::memory_order_seq_cst));
//	}
//
//	void push(const int & data)
//	{
//		Node * new_head = new Node(data, _head.load(rl::memory_order_seq_cst));
//
//		while (!_head.compare_exchange_strong(new_head->next, new_head, rl::memory_order_seq_cst))
//		{
//			; // empty loop
//		}
//	}
//
//	bool try_pop(int & data)
//	{
//		Node * old_head = _head.load(rl::memory_order_seq_cst);
//		if (old_head == nullptr)
//		{
//			return false;
//		}
//		//auto p = old_head($);
//		while (old_head && !_head.compare_exchange_strong(old_head, old_head->next, rl::memory_order_seq_cst))
//		{
//			; // empty loop
//		}
//
//		if (old_head == nullptr)
//			return false;
//
//		// if copy throws exception, then the value will be lost
//		data = old_head->data($);
//
//		// add to list
//		old_head->next = _to_delete.load(rl::memory_order_seq_cst);
//		while (!_to_delete.compare_exchange_strong(old_head->next, old_head, rl::memory_order_seq_cst))
//		{
//			; // empty loop
//		}
//		return true;
//	}
//
//private:
//	struct Node
//	{
//		Node * next = nullptr;
//
//		rl::var<int> data;
//
//		Node(const int& v)
//			: next(nullptr)
//			, data(v)
//		{ }
//
//		Node(const int& v, Node * n)
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
//	stack(stack const&);
//	stack& operator = (stack const&);
//};
//
//
//
//
//struct stack_test : rl::test_suite<stack_test, 4>
//{
//	stack s_;
//
//	int produced_count_;
//	int consumed_count_;
//	int false_count_;
//
//	void before()
//	{
//		produced_count_ = 0;
//		consumed_count_ = 0;
//		false_count_ = 0;
//	}
//
//	void after()
//	{
//		typedef rl::test_suite<stack_test, 4> base_t;
//		std::string s = std::to_string(false_count_);
//	}
//
//	void thread(unsigned index)
//	{
//		if (index % 3 == 0)
//		{
//			for (int i = 0; i < 10; i++)
//			{
//				s_.push(rand() + 1 * i);
//			}
//			produced_count_ += 1;
//		}
//		else
//		{
//			int data = 0;
//			if (!s_.try_pop(data))
//				false_count_++;
//			//RL_ASSERT(data);
//			consumed_count_ += 1;
//		}
//	}
//};
//
//
//
//
//int main()
//{
//	rl::simulate<stack_test>();
//}
//
