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

	~OrdonnanceurLocal();
	
	OrdonnanceurLocal(std::string host);

	static unsigned int GetNbThread();
	CPasswordChunk GetChunk();
	static unsigned int GetAvailableMemory();
	strCommandLine* getCommandLine(void);
	void RequestChunk();
	void StartThread();
	void CreateThread();
	std::string HashPassword(std::string passwordToHash);
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
	const std::string _PROTOSTART = "HELLO-HOW-SHOULD-I-WORK";
	const std::string _NEEDCHUNK = "NEW-CHUNK-PLEASE LAST-HANDLED-CHUNK=";
	const std::string _FOUNDANDEXIT = "FOUND-PLEASE-EXIT PASSWORD=x FOUND-BY=y";
	
	std::string _host;
	std::string _passwordATrouver;
	std::string _algo;
	std::string _alphabet;
	const std::string _patternAlgo = "ALGO=([^ ]+)";
	const std::string _patternHash = "HASH=([^ ]+)";
	const std::string _patternAlphabet = "ALPHABET=([^ ]+)";
	const std::string _patternChunk = "YOU=([^ ]+)";
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

