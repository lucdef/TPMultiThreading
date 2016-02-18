#include "OrdonnanceurLocal.h"
#include <thread>
#include <Windows.h>



OrdonnanceurLocal::OrdonnanceurLocal()
{
	bThreadLocal = OrdonnanceurLocal::GetNbThread() / 2;
	unsigned int sizeOfMemory = OrdonnanceurLocal::GetAvailableMemory();
	if (sizeOfMemory > SIZE_OF_CHUNK)
	{
		_aIdThread = new pthread_t[bThreadLocal];
		
	}

}

unsigned int OrdonnanceurLocal::GetNbThread()
{

	unsigned int nbThead = std::thread::hardware_concurrency();
	return nbThead;
}

CPasswordChunk OrdonnanceurLocal::GetChunk()
{
	if (_fifo.Count()<(bThreadLocal*2))
	{
		RequestChunk();

	}
	
	return _fifo.Pop();
	
}

unsigned int OrdonnanceurLocal::GetAvailableMemory()
{
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	return status.ullAvailPhys*0.9;
}

void OrdonnanceurLocal::RequestChunk()
{
	CPasswordChunk* test = new CPasswordChunk("aa", "cc");
	CPasswordChunk* test1 = new CPasswordChunk("cc", "ee");
	CPasswordChunk* test2 = new CPasswordChunk("dd", "ff");
	CPasswordChunk* test3 = new CPasswordChunk("gg", "ii");
	_fifo.Push(*test);
	_fifo.Push(*test1);
	_fifo.Push(*test2);
	_fifo.Push(*test3);

}

void OrdonnanceurLocal::StartThread()
{
	this->CreateThread();
	std::cout << "Test";
}

void OrdonnanceurLocal::CreateThread()
{
	_args = new paramThread;
	_args->instance = this;
	_args->arret = false;
	_args->passwordToFind = "bb";
	for (int i = 0; i < bThreadLocal; i++)
	{
		pthread_t idThread;

		void*(*ptr_GenPassword)(void *) = &Agent::GenerationPassword;
		pthread_attr_t paramThread;
		pthread_create(&idThread, nullptr, ptr_GenPassword, _args);
		_aIdThread[i] = idThread;
	}
}

void OrdonnanceurLocal::StopThread()
{
	if (_aIdThread != nullptr)
	{
		void* result = "";
		_args->arret = true;
		std::cout << "Arrêt en cours, merci de patientez" << std::endl;
		for (int i = 0; i < bThreadLocal; i++) {
			std::cout << "Arret en cours du thread" << i << std::endl;
			pthread_join(_aIdThread[i], &result);
			std::string* resutOfThread= reinterpret_cast<std::string*>(result);
			if (*resutOfThread != "")
			{
				std::cout << "Password trouvé: " << *resutOfThread << " par le thread " << i << std::endl;
			}
			std::cout << "Arret thread " << i << std::endl;
		}
		FreeRessources();
	}
}

int OrdonnanceurLocal::GetNbThreadLocal()
{
	return bThreadLocal;
}


void OrdonnanceurLocal::DecrementNBThread()
{
	std::cout << bThreadLocal << std::endl;
	bThreadLocal--;
}

void OrdonnanceurLocal::FreeRessources()
{
	delete _aIdThread;
	delete _args;
}

OrdonnanceurLocal::~OrdonnanceurLocal()
{
	
}

void OrdonnanceurLocal::setChunk(CPasswordChunk passwordChunk)
{
	_fifo.Push(passwordChunk);
}
