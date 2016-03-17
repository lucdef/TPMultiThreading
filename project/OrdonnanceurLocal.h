#pragma once
#include "PasswordChunk.h"
#include "FIFO.h"
#include "TrueMutex.hpp"

class OrdonnanceurLocal
{
public:

	struct strCommandLine
	{
		std::string hash;
		std::string algo;
		std::string alphabet;
		std::string masterIPAddress;
		unsigned int chunkSize;
		std::string OrdoLocal;
		std::string OrdoGlobal;
	};

	OrdonnanceurLocal();
	~OrdonnanceurLocal();
	
	static unsigned int GetNbThread();
	CPasswordChunk GetChunk();
	static unsigned int GetAvailableMemory();
	strCommandLine* getCommandLine(void);
	void RequestChunk();
	void StartThread();
	void CreateThread();
	void StopThread();
	int GetNbThreadLocal();
	void FreeRessources();
	void SetPasswordFind(std::string password);
	std::string getHost();
	
	
	struct paramThread
	{
		OrdonnanceurLocal* instance;
		bool arret;
		std::string passwordToFind;
	};

private:
	//FIFO de chunk
	std::string _host;
	FIFO<CPasswordChunk> _fifo;
	//passage d'argument 
	paramThread* _args;
	strCommandLine* _CommandLine;
	std::string _passwordFind;
	unsigned int bThreadLocal;
	void setChunk(CPasswordChunk passwordChunk);
	TrueMutex _mutex;
	pthread_t* _aIdThread;
	unsigned int const SIZE_OF_CHUNK = 128;

};

