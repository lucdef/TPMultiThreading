#pragma once
#include <queue>


template <class T>
class Fifo<T> : public std::queue<T>
{
public:
	Fifo<T>();
	~Fifo<T>();
};


