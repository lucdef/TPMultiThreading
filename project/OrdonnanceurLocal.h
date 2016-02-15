#pragma once
#include "PasswordChunk.h"
#include "FIFO.h"
class OrdonnanceurLocal
{
public:
	OrdonnanceurLocal();
	static unsigned int GetNbThread();
	CPasswordChunk GetChunk();
	static unsigned int GetAvailableMemory();
	void RequestChunk();
	void StartChunk();
	void CreateThread();
	void StopThread();
	~OrdonnanceurLocal();
private:
	FIFO<CPasswordChunk> _fifo;
	void setChunk(CPasswordChunk passwordChunk);
	pthread_t* _aIdThread;
	unsigned int const SIZE_OF_CHUNK = 128;

};

