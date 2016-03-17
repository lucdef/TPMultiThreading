#include "OrdonnanceurLocal.h"
#include <thread>
#include <Windows.h>
#include "TrueMutex.hpp"
#include "Agent.h"
#include "TcpClient.hpp"
#include "TcpServer.hpp";
#include "utils.h"

OrdonnanceurLocal::OrdonnanceurLocal(std::string host)
{
	_CommandLine = new strCommandLine;
	bThreadLocal = OrdonnanceurLocal::GetNbThread() / 2;
	bThreadLocal = 1;
	unsigned int sizeOfMemory = OrdonnanceurLocal::GetAvailableMemory();
	_mutex.Init();
	//Sécurité
	if (sizeOfMemory > SIZE_OF_CHUNK)
	{
		_aIdThread = new pthread_t[bThreadLocal];
		
	}
	this->_host = host;
	TcpClient clientTcp;
	clientTcp.ConnectToHost(this->getHost());
	clientTcp.SendHttpRequest(this->getHost(), this->_PROTOSTART);
	clientTcp.WaitForResponse();
	std::string tmp = clientTcp.GetResponse();
	clientTcp.CloseConnection();
	this->_algo = Utils::GetPatternFromData(tmp,this->_patternAlgo);
	this->_alphabet = Utils::GetPatternFromData(tmp, this->_patternAlphabet);
	this->_passwordATrouver = Utils::GetPatternFromData(tmp, this->_patternHash);
	
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
	CPasswordChunk* test = new CPasswordChunk("00", "zz");
	TcpClient tcpClient =  TcpClient();
	tcpClient.ConnectToHost(this->getHost());
	std::string needChunk = "000000000";
	tcpClient.SendHttpRequest(this->getHost(), needChunk);
	tcpClient.WaitForResponse();
	tcpClient.GetResponse();
	tcpClient.CloseConnection();
	_fifo.Push(*test);
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
	_args->passwordToFind = "afi";
	for (int i = 0; i < bThreadLocal; i++)
	{
		pthread_t idThread;

		void*(*ptr_GenPassword)(void *) = &Agent::GenerationPassword;
		//pthread_attr_t paramThread;
		pthread_create(&idThread, nullptr, ptr_GenPassword, _args);
		_aIdThread[i] = idThread;
	}
}

void OrdonnanceurLocal::StopThread()
{
	if (_aIdThread != nullptr)
	{
		
		_args->arret = true;
		std::cout << "Arrêt en cours, merci de patientez" << std::endl;
		for (int i = 0; i < bThreadLocal; i++) {
			std::cout << "Arret en cours du thread" << i << std::endl;
			pthread_join(_aIdThread[i], nullptr);
			
			std::cout << "Arret thread " << i << std::endl;
		}
		if (_passwordFind != "")
		{
			std::cout << "Password trouvé: " << _passwordFind  << std::endl;
		}
		FreeRessources();
	}
}

int OrdonnanceurLocal::GetNbThreadLocal()
{
	return bThreadLocal;
}

void OrdonnanceurLocal::FreeRessources()
{
	delete _aIdThread;
	delete _args;
	delete _CommandLine;
}

void OrdonnanceurLocal::SetPasswordFind(std::string password)
{
	_mutex.Lock();
	_passwordFind = password;
	_mutex.Unlock();
}

std::string OrdonnanceurLocal::getHost()
{
	return this->_host;
}

OrdonnanceurLocal::~OrdonnanceurLocal()
{
	
}

OrdonnanceurLocal::strCommandLine* OrdonnanceurLocal::getCommandLine(void)
{
	return _CommandLine;
}

void OrdonnanceurLocal::setChunk(CPasswordChunk passwordChunk)
{
	_fifo.Push(passwordChunk);
}
