#include "OrdonnanceurLocal.h"
#include <thread>
#include <Windows.h>
#include "TrueMutex.hpp"
#include "Agent.h"
#include "TcpClient.hpp"
#include "TcpServer.hpp";
#include "utils.h"
#include "HashUtils.h"
#include "LogManager.h"

#include <sstream>

OrdonnanceurLocal::OrdonnanceurLocal(std::string host)
{
	_CommandLine = new strCommandLine;
	bThreadLocal = OrdonnanceurLocal::GetNbThread();
	
	unsigned int sizeOfMemory = OrdonnanceurLocal::GetAvailableMemory();
	_mutex.Init();

	//S�curit�
	if (sizeOfMemory > SIZE_OF_CHUNK)
	{
		_aIdThread = new pthread_t[bThreadLocal];	
	}

	this->_host = host;
	TcpClient clientTcp;
	clientTcp.ConnectToHost(this->getHost(),666);
	clientTcp.SendHttpRequest(this->getHost(), this->_PROTOSTART);
	clientTcp.WaitForResponse();
	clientTcp.ReceiveResponse(true);
	std::string tmp = clientTcp.GetResponse();
	clientTcp.CloseConnection();
	this->_algo = Utils::GetPatternFromData(tmp, Utils::_patternAlgo);
	this->_alphabet = Utils::GetPatternFromData(tmp, Utils::_patternAlphabet);	this->_passwordATrouver = Utils::GetPatternFromData(tmp, Utils::_patternHash);
}

unsigned int OrdonnanceurLocal::GetNbThread()
{
	unsigned int nbThead = std::thread::hardware_concurrency();
	return nbThead;
}

CPasswordChunk OrdonnanceurLocal::GetChunk()
{
	int nbChunkInFifo = _fifo.Count();

	if (nbChunkInFifo<(bThreadLocal*2))
	{
		CPasswordChunk lastChunk;
		std::ostringstream chunkstringifier;
		if (nbChunkInFifo != 0)
		{
			lastChunk = this->_fifo.GetLastChunk();
			chunkstringifier << lastChunk.GetPasswordBegin() << "|" << lastChunk.GetPasswordEnd();
		}
		else
		{
			chunkstringifier << "";
		}
		RequestChunk((bThreadLocal*2-nbChunkInFifo),chunkstringifier.str());
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

void OrdonnanceurLocal::RequestChunk(int nbRequest,std::string lastHandle)
{
	LogManager::GetInstance()->LogInfo(1, "Need Some Chunk over here");
	for (int i=0; i < nbRequest; i++) {
		TcpClient tcpClient = TcpClient();
		tcpClient.ConnectToHost(this->getHost(), 666);

		 std::ostringstream needChunk;
		needChunk<<"NEW-CHUNK-PLEASE LAST-HANDLED-CHUNK=" << lastHandle;

		tcpClient.SendHttpRequest(this->getHost(), needChunk.str());
		tcpClient.WaitForResponse();
		tcpClient.ReceiveResponse(true);
		std::string reponse = tcpClient.GetResponse();
		std::string delimiter = "|";
		std::string chunk = Utils::GetPatternFromData(reponse, Utils::_patternChunk);
		std::string startpass = chunk.substr(0, chunk.find(delimiter));
		std::string endpass = chunk.substr(chunk.find(delimiter) + 1, std::string::npos);
		CPasswordChunk giveMeSomeChunk(startpass, endpass);
		tcpClient.CloseConnection();
		_fifo.Push(giveMeSomeChunk);
	}
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
	_args->passwordToFind = this->_passwordATrouver;
	for (int i = 0; i < bThreadLocal; i++)
	{
		pthread_t idThread;

		void*(*ptr_GenPassword)(void *) = &Agent::GenerationPassword;
		//pthread_attr_t paramThread;
		pthread_create(&idThread, nullptr, ptr_GenPassword, _args);
		_aIdThread[i] = idThread;
	}
}
std::string OrdonnanceurLocal::HashPassword(std::string passwordToHash)
{
	std::string passwordHasher = passwordToHash;
	std::string algo = this->_algo;
	if (algo == "sha256") {
		passwordHasher = HashUtils::GetInstance()->HashSha256(passwordToHash);
	}
	else if (algo == "sha1")
	{
		passwordHasher = HashUtils::GetInstance()->HashSha1(passwordToHash);
	}
	else if (algo == "sha224")
	{
		passwordHasher = HashUtils::GetInstance()->HashSha224(passwordToHash);
	}
	else if (algo == "md5")
	{
		passwordHasher = HashUtils::GetInstance()->hashMd5(passwordToHash);
	}
	else if (algo == "sha224")
	{
		passwordHasher = HashUtils::GetInstance()->HashCrc32(passwordToHash);
	}
	return passwordHasher;
		
	
}
void OrdonnanceurLocal::StopThread()
{
	if (_aIdThread != nullptr)
	{
		
		_args->arret = true;
		std::cout << "Arr�t en cours, merci de patienter" << std::endl;
		
		
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
	std::cout << "je me detrui" << std::endl;
}

OrdonnanceurLocal::strCommandLine* OrdonnanceurLocal::getCommandLine(void)
{
	return _CommandLine;
}

void OrdonnanceurLocal::setChunk(CPasswordChunk passwordChunk)
{
	_fifo.Push(passwordChunk);
}
std::string OrdonnanceurLocal::GetAlphabet()
{
	return this->_alphabet;
}
void OrdonnanceurLocal::FoundPassword(std::string passwordFound)
{
	TcpClient tcpClient;
	tcpClient.ConnectToHost(this->_host, 666);
	std::ostringstream request;
		request << this->_FOUNDANDEXIT << passwordFound;
	
	tcpClient.SendHttpRequest(this->_host, request.str());
	tcpClient.WaitForResponse();
	tcpClient.ReceiveResponse(true);
	tcpClient.CloseConnection();
	this->StopThread();


}
void OrdonnanceurLocal::WaitThreads()
{
	for (int i = 0; i < bThreadLocal; i++) {
		if(_aIdThread!=nullptr){
		std::cout << "Arret en cours du thread" << i << std::endl;
		pthread_join(_aIdThread[i], nullptr);
		}

		std::cout << "Arret thread " << i << std::endl;
		LogManager::GetInstance()->LogInfo(i, "Arret du thread");

	}
	FreeRessources();
}

void OrdonnanceurLocal::Run()
{
	StartThread();
	WaitThreads();
}