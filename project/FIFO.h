#pragma once
#include <cstdlib>
#include "stdafx.h"
#include <iostream>
#include<queue>
#include<deque>
#include "TrueMutex.hpp"
#include <pthread.h>
template <typename T> class FIFO
{
public:
	FIFO()
	{
		_mutex  = PTHREAD_MUTEX_INITIALIZER;
	}
	T Pop()
	{
		//_mutex->Lock();
		T firstElement = _list.at(0);
		_list.pop_front();
		//_mutex->UnLock();
		return firstElement;
	}
	
	void Push(T endElement)
	{
		//_mutex->Lock();
		_list.push_back(endElement);
		//_mutex->UnLock();
	}
	void Clear()
	{
		_mutex->Lock();
		_list.clear();
		//_mutex->UnLock();
	}
	int Count()
	{
		pthread_mutex_lock(&_mutex);
		int sizeOfFifo = _list.size();
		pthread_mutex_unlock(&_mutex);
		return sizeOfFifo;
	}

	~FIFO()
	{
		delete _mutex;
	}
private:
	std::deque<T> _list;
	pthread_mutex_t _mutex;

};

