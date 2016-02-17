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

	public:
		OGlobal(const int nbThread, const int chunkSize, const std::string algo, const std::string hash = "XXXXXXXXXX", const std::string alphabet = "0123456789");
		~OGlobal();
		const std::string GetAlphabet() const;
		const std::string GetAlgo() const;
		const std::string GetHash() const;
		const std::string GetNextChunkBegin() const;

		// TODO: make private
		std::string generateChunk(const  std::string begin);
		std::string getBeginFromEnd(const  std::string end);
		int IndexOf(char letter);

		//void replyChunk();



	private:
		TcpServer _server;
		CPasswordChunk _nextChunk;
		std::deque<givenChunk_t> _givenChunks;

		const std::string _hash;
		const std::string _alphabet;
		const int _chunkSize;
		const std::string _algo;



		//int _threadIds[THREAD_COUNT]; // TODO: change max
		//pthread_t _threads[THREAD_COUNT];

		//void createThreads();
		//void *threadFunction()
};

