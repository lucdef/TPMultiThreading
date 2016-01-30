#pragma once

#include <pthread.h>


class TrueMutex
{
	public:
		TrueMutex();
		virtual ~TrueMutex();
		void Init();
		void Lock();
		void Unlock();

	private:
		pthread_mutex_t _mutex;
};

