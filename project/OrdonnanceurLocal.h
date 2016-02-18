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
	~OrdonnanceurLocal();
private:
	FIFO<CPasswordChunk> _fifo;
	unsigned int nbThreadLocal;
	void setChunk(CPasswordChunk passwordChunk);
	pthread_t* _aIdThread;
	unsigned int const SIZE_OF_CHUNK = 128;

};

