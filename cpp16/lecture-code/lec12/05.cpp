//
//// care!!! this code supports only Windows OS
//// to crossplatfort seee http://www.everfall.com/paste/id.php?59ndmq1wqj54
//
//#include <intrin.h>
//#include <atomic>
//
//template <class T>
//class TLockFreeQueue
//{
//	struct TListNode
//	{
//		TListNode *volatile Next;
//		T Data;
//	};
//
//	struct TRootNode
//	{
//		TListNode *volatile PushQueue;
//		TListNode *volatile PopQueue;
//		TListNode *volatile ToDelete;
//		TRootNode *volatile NextFree;
//
//		TRootNode() : PushQueue(0), PopQueue(0), ToDelete(0), NextFree(0) { }
//	};
//
//	static void EraseList(TListNode *n)
//	{
//		while (n) {
//			TListNode *keepNext = n->Next;
//			delete n;
//			n = keepNext;
//		}
//	}
//
//	TRootNode *volatile JobQueue;
//	volatile long FreememCounter;
//	TRootNode *volatile FreePtr;
//
//	static bool cas(intptr_t * a, intptr_t exchange, intptr_t compare)
//	{
//		return _InterlockedCompareExchange((volatile long*)a, exchange, compare) == compare;
//	}
//
//	void TryToFreeAsyncMemory()
//	{
//		TRootNode *current = FreePtr;
//		if (current == 0)
//			return;
//		
//		if (_InterlockedExchangeAdd((volatile long*)&FreememCounter, 0) == 1) {
//			// we are the last thread, try to cleanup
//			if (cas((intptr_t *)(&FreePtr),	reinterpret_cast<intptr_t>(nullptr), reinterpret_cast<intptr_t>(current)))
//			{
//				// free list
//				while (current) {
//					TRootNode *p = current->NextFree;
//					EraseList(current->ToDelete);
//					delete current;
//					current = p;
//				}
//			}
//		}
//	}
//
//	void AsyncRef()
//	{
//		_InterlockedIncrement((volatile long*)&FreememCounter);
//	}
//	void AsyncUnref()
//	{
//		TryToFreeAsyncMemory();
//		_InterlockedDecrement((volatile long*)&FreememCounter);
//	}
//	void AsyncDel(TRootNode *toDelete, TListNode *lst)
//	{
//		toDelete->ToDelete = lst;
//		for (;;) {
//			toDelete->NextFree = FreePtr;
//			if (cas((intptr_t *)(&FreePtr), reinterpret_cast<intptr_t>(toDelete), reinterpret_cast<intptr_t>(toDelete->NextFree)))
//				break;
//		}
//	}
//	void AsyncUnref(TRootNode *toDelete, TListNode *lst)
//	{
//		TryToFreeAsyncMemory();
//		if (_InterlockedDecrement((volatile long*)&FreememCounter) == 0) {
//			// no other operations in progress, can safely reclaim memory
//			EraseList(lst);
//			delete toDelete;
//		}
//		else {
//			// Dequeue()s in progress, put node to free list
//			AsyncDel(toDelete, lst);
//		}
//	}
//
//
//	struct TListInvertor
//	{
//		TListNode *Copy;
//		TListNode *Tail;
//		TListNode *PrevFirst;
//
//		TListInvertor() : Copy(0), Tail(0), PrevFirst(0) { }
//		~TListInvertor()
//		{
//			if (Copy)
//				Copy = Copy;
//			EraseList(Copy);
//		}
//		void CopyWasUsed()
//		{
//			Copy = 0;
//			Tail = 0;
//			PrevFirst = 0;
//		}
//		void DoCopy(TListNode *ptr)
//		{
//			TListNode *newFirst = ptr;
//			TListNode *newCopy = 0;
//			TListNode *newTail = 0;
//			while (ptr) {
//				if (ptr == PrevFirst) {
//					// short cut, we have copied this part already
//					Tail->Next = newCopy;
//					newCopy = Copy;
//					Copy = 0; // do not destroy prev try
//					if (!newTail)
//						newTail = Tail; // tried to invert same list
//					break;
//				}
//				TListNode *newElem = new TListNode;
//				newElem->Data = ptr->Data;
//				newElem->Next = newCopy;
//				newCopy = newElem;
//				ptr = ptr->Next;
//				if (!newTail)
//					newTail = newElem;
//			}
//			EraseList(Copy); // copy was useless
//			Copy = newCopy;
//			PrevFirst = newFirst;
//			Tail = newTail;
//		}
//	};
//
//	TLockFreeQueue(const TLockFreeQueue&) { }
//	void operator=(const TLockFreeQueue&) { }
//public:
//	TLockFreeQueue() : JobQueue(new TRootNode), FreememCounter(0), FreePtr(0) { }
//	~TLockFreeQueue()
//	{
//		AsyncRef();
//		AsyncUnref();
//		EraseList(JobQueue->PushQueue);
//		EraseList(JobQueue->PopQueue);
//		delete JobQueue;
//	}
//	void Enqueue(const T &data)
//	{
//		TListNode *newNode = new TListNode;
//		newNode->Data = data;
//		TRootNode *newRoot = new TRootNode;
//		AsyncRef();
//		newRoot->PushQueue = newNode;
//		for (;;) {
//			TRootNode *curRoot = JobQueue;
//			newRoot->PushQueue = newNode;
//			newNode->Next = curRoot->PushQueue;
//			newRoot->PopQueue = curRoot->PopQueue;
//			if (cas((intptr_t *)(&JobQueue), reinterpret_cast<intptr_t>(newRoot), reinterpret_cast<intptr_t>(curRoot)))
//			{
//				AsyncUnref(curRoot, 0);
//				break;
//			}
//		}
//	}
//	bool Dequeue(T *data)
//	{
//		TRootNode *newRoot = 0;
//		TListInvertor listInvertor;
//		AsyncRef();
//		for (;;) {
//			TRootNode *curRoot = JobQueue;
//			TListNode *tail = curRoot->PopQueue;
//			if (tail) {
//				// has elems to pop
//				if (!newRoot)
//					newRoot = new TRootNode;
//
//				newRoot->PushQueue = curRoot->PushQueue;
//				newRoot->PopQueue = tail->Next;
////				ASSERT(curRoot->PopQueue == tail);
//				if (cas((intptr_t *)(&JobQueue), reinterpret_cast<intptr_t>(newRoot), reinterpret_cast<intptr_t>(curRoot))) {
//					*data = tail->Data;
//					tail->Next = 0;
//					AsyncUnref(curRoot, tail);
//					return true;
//				}
//				continue;
//			}
//			if (curRoot->PushQueue == 0) {
//				delete newRoot;
//				AsyncUnref();
//				return false; // no elems to pop
//			}
//
//			if (!newRoot)
//				newRoot = new TRootNode;
//			newRoot->PushQueue = 0;
//			listInvertor.DoCopy(curRoot->PushQueue);
//			newRoot->PopQueue = listInvertor.Copy;
////			ASSERT(curRoot->PopQueue == 0);
//			if (cas((intptr_t*)&JobQueue, reinterpret_cast<intptr_t>(newRoot), reinterpret_cast<intptr_t>(curRoot)))
//			{
//				newRoot = 0;
//				listInvertor.CopyWasUsed();
//				AsyncDel(curRoot, curRoot->PushQueue);
//			}
//			else {
//				newRoot->PopQueue = 0;
//			}
//		}
//	}
//	bool IsEmpty()
//	{
//		AsyncRef();
//		TRootNode *curRoot = JobQueue;
//		bool res = curRoot->PushQueue == 0 && curRoot->PopQueue == 0;
//		AsyncUnref();
//		return res;
//	}
//};
//
//int main()
//{
//	TLockFreeQueue<int> stack;
//
//	int i = 0;
//	stack.Enqueue(99);
//
//	stack.Enqueue(1);
//	stack.Enqueue(2);
//
//	stack.Dequeue(&i);
//	stack.Dequeue(&i);
//	stack.Dequeue(&i);
//	stack.Dequeue(&i);
//
//
//	return 0;
//
//}
