#pragma once

#include <deque>
#include "pthread.h"
#include "TcpServer.hpp"
#include "PasswordChunk.h"

#define THREAD_COUNT 5



class OGlobal
{
	struct givenChunk_t {
		std::string startPass;
		std::string agentIp;
		int			port;
	};

	struct keyboardThreadArgs_t {
		bool *appRunning;
	};

public:
	static OGlobal* GetInstance(const int nbThread = -1, const int chunkSize = -1, const std::string algo = "", const std::string hash = "", const std::string alphabet = "");
	static void Kill();

	const std::string GetAlphabet() const;
	const std::string GetAlgo() const;
	const std::string GetHash() const;
	const std::string GetNextChunkBegin() const;
	const std::string CraftResponse(const std::string request);
	void StartServer(int port);
	void StartKeyboardThread(bool isBlocking = true);
	void StartServerThread();
	void Run();


	// TODO: make private
	std::string generateChunk(const  std::string begin);
	std::string getBeginFromEnd(const  std::string end);
	int IndexOf(char letter);

	//void replyChunk();



private:
	static OGlobal *_instance;

	const std::string _hash;
	const std::string _alphabet;
	const int _chunkSize;
	const std::string _algo;

	bool _appRunning;
	TcpServer _server;
	CPasswordChunk _nextChunk;
	std::deque<givenChunk_t> _givenChunks;
	pthread_t _keyboardThread;
	pthread_t _serverThread;


	OGlobal(const int nbThread, const int chunkSize, const std::string algo, const std::string hash, const std::string alphabet);
	OGlobal& operator= (const OGlobal&) {} // TODO
	~OGlobal();
	static void *ThreadKeyboardFunc(void *p_arg);
	static void *ThreadServerFunc(void *p_arg);




	//OGlobal(const OGlobal&) {} //TODO



	//int _threadIds[THREAD_COUNT]; // TODO: change max
	//pthread_t _threads[THREAD_COUNT];

	//void createThreads();
	//void *threadFunction()
};

