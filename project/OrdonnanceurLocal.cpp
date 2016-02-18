#include "OrdonnanceurLocal.h"
#include <thread>
#include <Windows.h>



OrdonnanceurLocal::OrdonnanceurLocal()
{
	unsigned int nbThreadLocal = OrdonnanceurLocal::GetNbThread() / 2;
	unsigned int sizeOfMemory = OrdonnanceurLocal::GetAvailableMemory();
	if (sizeOfMemory > SIZE_OF_CHUNK)
	{
	_aIdThread = new pthread_t[nbThreadLocal];
		for (int i = 0; i < nbThreadLocal; i++)
		{
			pthread_t idThread;
			CPasswordChunk (OrdonnanceurLocal::*ptr_GetChunk)()=&OrdonnanceurLocal::GetChunk;
			std::cout << "";
			int test;
			std::cin >> test;
	
			//void *p = (this->GetChunk())();
			pthread_attr_t paramThread;	
			//pthread_create(&idThread)
		}
	}

}

unsigned int OrdonnanceurLocal::GetNbThread()
{

	unsigned int nbThead = std::thread::hardware_concurrency();
	return nbThead;
}

CPasswordChunk OrdonnanceurLocal::GetChunk()
{
		return _fifo.Pop();
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
		args->passwordToFind = "aaaaa";
		void*(*ptr_GenPassword)(void *) = &Agent::GenerationPassword;
		pthread_attr_t paramThread;
		pthread_create(&idThread, nullptr, ptr_GenPassword, args);
		_aIdThread[i] = idThread;
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
