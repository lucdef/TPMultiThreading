#pragma once

#include <deque>
#include "pthread.h"
#include "TcpServer.hpp"
#include "PasswordChunk.h"
#include "LogManager.h"

#define THREAD_COUNT 5

class CSocket;

class OGlobal
{
	struct givenChunk_t
	{
		std::string startPass;
		std::string agentIp;
		int			port;
	};

public:
	static OGlobal* GetInstance(const int nbThread = -1, const int chunkSize = -1, const std::string algo = "", const std::string hash = "", const std::string alphabet = "");
	static void Kill();

	std::string GetAlphabet() const;
	std::string GetAlgo() const;
	std::string GetHash() const;
	CPasswordChunk GetNextChunk();
	void StartServer(int port);
	void StartKeyboardThread(const bool isBlocking);
	void StartServerThread();
	void AddGivenChunk(std::string startPass, CSocket *client);
	void Run();

	// TODO: make private
	std::string generateChunk(const  std::string begin);
	std::string getBeginFromEnd(std::string end);
	int IndexOf(const char letter) const;
	unsigned int GetChunkSize() const;
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
	LogManager *_logger;

	OGlobal(const int nbThread, const int chunkSize, const std::string algo, const std::string hash, const std::string alphabet);
	~OGlobal();
	void initNextChunk();
	static void *ThreadKeyboardFunc(void *p_arg);
	static void *ThreadServerFunc(void *p_arg);
};

