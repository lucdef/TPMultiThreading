#pragma once
#include <cstdlib>
#include "stdafx.h"
#include <iostream>
#include<queue>
#include<deque>
#include "TrueMutex.hpp"
template <typename T> class FIFO
{
public:
	FIFO()
	{
		_mutex = new TrueMutex();
		_mutex->Init();
	}
	T Pop()
	{
		_mutex->Lock();
		T firstElement = _list.at(0);
		_list.pop_front();
		_mutex->Unlock();
		return firstElement;
	}
	
	void Push(T endElement)
	{
		_mutex->Lock();
		_list.push_back(endElement);
		_mutex->Unlock();
	}
	void Clear()
	{
		_mutex->Lock();
		_list.clear();
		_mutex->Unlock();
	}
	int Count()
	{
		_mutex->Lock();
		int sizeOfFifo = _list.size();
		_mutex->Unlock();
		return sizeOfFifo;
	}

	~FIFO()
	{
		delete _mutex;
	}
private:
	std::deque<T> _list;
	TrueMutex* _mutex;

};

