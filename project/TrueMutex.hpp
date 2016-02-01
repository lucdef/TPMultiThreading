#pragma once

#if (_MSC_VER >= 1900)
#define _TIMESPEC_DEFINED
#endif
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

