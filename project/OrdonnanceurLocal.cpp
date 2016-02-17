#include "OrdonnanceurLocal.h"
#include <pthread.h>
#include <thread>
#include <Windows.h>
#include "Agent.h"



OrdonnanceurLocal::OrdonnanceurLocal()
{
	 this->nbThreadLocal = OrdonnanceurLocal::GetNbThread() / 2;
	unsigned int sizeOfMemory = OrdonnanceurLocal::GetAvailableMemory();
	_aIdThread = new pthread_t[nbThreadLocal];
	

}

unsigned int OrdonnanceurLocal::GetNbThread()
{

	unsigned int nbThead = std::thread::hardware_concurrency();
	return nbThead;
}

CPasswordChunk OrdonnanceurLocal::GetChunk()
{
	if (_fifo.Count() < this->nbThreadLocal + 1)
	{
		return CPasswordChunk("aaaa", "bbbbb");
	}
	else
	{
		return _fifo.Pop();
	}
}

unsigned int OrdonnanceurLocal::GetAvailableMemory()
{
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	return status.ullAvailPhys*0.9;
}

void OrdonnanceurLocal::StartThread()
{
	this->CreateThread();
	std::cout << "Test";
}

void OrdonnanceurLocal::CreateThread()
{
	
	for (int i = 0; i < nbThreadLocal; i++)
	{
		pthread_t idThread;
		Agent::paramThread *args = new Agent::paramThread;
		args->instance = this;
		args->arret = false;
		args->passwordToFind = "bbaa";
		void*(*ptr_GenPassword)(void *) = &Agent::GenerationPassword;
		pthread_attr_t paramThread;
		pthread_create(&idThread, nullptr, ptr_GenPassword, args);
		_aIdThread[i] = idThread;
	}
}

void OrdonnanceurLocal::StopThread()
{
	for (int i = 0; i < sizeof(_aIdThread);i++)
	{
		pthread_join(_aIdThread[i], nullptr);
	}
	
}


OrdonnanceurLocal::~OrdonnanceurLocal()
{
	delete _aIdThread;
}

void OrdonnanceurLocal::setChunk(CPasswordChunk passwordChunk)
{
	_fifo.Push(passwordChunk);
}
