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

void TrueMutex::Lock()
{
	pthread_mutex_lock(&_mutex);
}

void TrueMutex::Unlock()
{
	pthread_mutex_unlock(&_mutex);
}
