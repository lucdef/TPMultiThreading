#include "TrueMutex.hpp"



TrueMutex::TrueMutex()
{
}


TrueMutex::~TrueMutex()
{
	pthread_mutex_destroy(&_mutex);
}

void TrueMutex::Init()
{
	_mutex = PTHREAD_MUTEX_INITIALIZER;
}

int TrueMutex::Lock()
{
	return pthread_mutex_lock(&_mutex);
}

int TrueMutex::Unlock()
{
	return pthread_mutex_unlock(&_mutex);
}
