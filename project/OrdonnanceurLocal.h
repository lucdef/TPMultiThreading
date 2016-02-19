#pragma once
#include "PasswordChunk.h"
#include "FIFO.h"
#include "Agent.h"
#include "TrueMutex.hpp"
class OrdonnanceurLocal
{
public:
	OrdonnanceurLocal();
	static unsigned int GetNbThread();
	CPasswordChunk GetChunk();
	static unsigned int GetAvailableMemory();
	void RequestChunk();
	void StartThread();
	void CreateThread();
	void StopThread();
	int GetNbThreadLocal();
	void FreeRessources();
	void SetPasswordFind(std::string password);
	~OrdonnanceurLocal();
	struct paramThread
	{
		OrdonnanceurLocal* instance;
		bool arret;
		std::string passwordToFind;
	};
private:
	//FIFO de chunk
	FIFO<CPasswordChunk> _fifo;
	//passage d'argument 
	paramThread* _args;
	std::string _passwordFind;
	unsigned int bThreadLocal;
	void setChunk(CPasswordChunk passwordChunk);
	TrueMutex _mutex;
	pthread_t* _aIdThread;
	unsigned int const SIZE_OF_CHUNK = 128;

};

