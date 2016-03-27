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

		pthread_mutex_lock(&_mutex);
		T firstElement = _list.at(0);
		_list.pop_front();

		pthread_mutex_unlock(&_mutex);
		return firstElement;
	}
	
	void Push(T endElement)
	{
		pthread_mutex_lock(&_mutex);
		_list.push_back(endElement);

		pthread_mutex_unlock(&_mutex);
	}
	void Clear()
	{

		pthread_mutex_lock(&_mutex);
		_list.clear();

		pthread_mutex_unlock(&_mutex);
	}
	int Count()
	{
		pthread_mutex_lock(&_mutex);
		int sizeOfFifo = _list.size();
		pthread_mutex_unlock(&_mutex);
		return sizeOfFifo;
	}
	T GetLastChunk()
	{
		
		pthread_mutex_lock(&_mutex);
		T element =  _list.at(_list.size() - 1);
		pthread_mutex_unlock(&_mutex);
		return element;
	}

	~FIFO()
	{
		delete _mutex;
	}
private:
	std::deque<T> _list;
	pthread_mutex_t _mutex;

};

