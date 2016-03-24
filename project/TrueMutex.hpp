#pragma once

#include <pthread.h>


class TrueMutex
{
	public:
		TrueMutex();
		virtual ~TrueMutex();
		void Init();
		int Lock();
		int Unlock();
private:
		pthread_mutex_t _mutex;
};

