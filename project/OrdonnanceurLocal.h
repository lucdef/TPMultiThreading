#pragma once
#include "PasswordChunk.h"
#include "FIFO.h"
#include "Agent.h"
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
	void DecrementNBThread();
	void FreeRessources();
	~OrdonnanceurLocal();
	struct paramThread
	{
		OrdonnanceurLocal* instance;
		bool arret;
		std::string passwordToFind;
	};
private:
	FIFO<CPasswordChunk> _fifo;
	paramThread* _args;
	unsigned int bThreadLocal;
	void setChunk(CPasswordChunk passwordChunk);
	pthread_t* _aIdThread;
	unsigned int const SIZE_OF_CHUNK = 128;

};

