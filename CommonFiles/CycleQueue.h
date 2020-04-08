#pragma once
#include <exception>

template <class T>
class CCycleQueue
{
public:
	CCycleQueue(size_t capacity) : _capacity(capacity)
	{
		clear();
		_pData = new T[_capacity];
	}

	~CCycleQueue()
	{
		if (_pData != nullptr)
		{
			delete[] _pData;
			_pData = nullptr;
		}
	}

	void clear()
	{
		_front = 0;
		_rear = 0;
		_count = 0;
	}

	size_t size() const
	{
		return _count;
	}

	bool empty() const
	{
		return (_count == 0);
	}

	bool full() const
	{
		return (_count == _capacity);
	}

	bool push(const T& theElement)
	{
		if (push_void())
		{
			back() = theElement;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool push_void()
	{
		if (full())
		{
			return false;
		}

		_rear = (_rear + 1) % _capacity;
		_count++;
		return true;
	}

	bool pop()
	{
		if (empty())
		{
			return false;
		}
		_front = (_front + 1) % _capacity;
		_count--;
		return true;
	}

	T& front()
	{
		if (empty())
		{
			throw std::exception("CycleQueue is empty, CCycleQueue::front() fail");
		}
		return _pData[_front];
	}

	T& back()
	{
		if (empty())
		{
			throw std::exception("CycleQueue is empty, CCycleQueue::back() fail");
		}
		size_t temp = (_rear + _capacity - 1) % _capacity;
		return _pData[temp];
	}

	/*size_t backIndex()
	{
		if (empty())
		{
			throw std::exception("CycleQueue is empty, CCycleQueue::backIndex() fail");
		}
		size_t temp = (_rear + _capacity - 1) % _capacity;
		return temp;
	}*/


protected:
	T*     _pData;			//数据的指针
	size_t _capacity;	//容量
	size_t _front;		//队头索引
	size_t _rear;		//队尾索引
	size_t _count;		//有效元素个数，计数器
};
