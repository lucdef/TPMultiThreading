#include "OGlobal.hpp"
#include "utils.h"
#include "conio.h"
#include "CSocket.h"
#define PORT_TCP	666
OGlobal* OGlobal::_instance = nullptr;

OGlobal::OGlobal(const int nbThread, const int chunkSize, const std::string algo, const std::string hash, const std::string alphabet)
	: _server(),
	_algo(algo),
	_appRunning(true),
	_hash(hash),
	_alphabet(alphabet),
	_chunkSize(chunkSize),
	_nextChunk()
{
	_logger = LogManager::GetInstance();

	// init next chunk
	initNextChunk();

	// init threadId and thread array
	//for (int i = 0; i < 5; ++i)
	//{
	//	_threadIds[i] = 0;
	//	//_threads[i] = ;
	//}

	//createThreads();
}

OGlobal::~OGlobal()
{
}

OGlobal * OGlobal::GetInstance(const int nbThread, const int chunkSize, const std::string algo, const std::string hash, const std::string alphabet)
{
	if (_instance == nullptr && nbThread != -1)
		_instance = new OGlobal(nbThread, chunkSize, algo, hash, alphabet);

	return _instance;
}

void OGlobal::Kill()
{
	delete(_instance);
	_instance = nullptr;
}


void OGlobal::AddGivenChunk(std::string startPass, CSocket *client)
{
	givenChunk_t given;
	given.startPass = startPass;
	given.port = client->GetRemoteEndpointPort();
	given.agentIp = client->GetRemoteEndpointIp();

	_givenChunks.push_back(given);
}

std::string OGlobal::GetAlphabet() const
{
	return _alphabet;
}

std::string OGlobal::GetAlgo() const
{
	return _algo;
}

std::string OGlobal::GetHash() const
{
	return _hash;
}

CPasswordChunk OGlobal::GetNextChunk()
{
	// celui stock� et le bon...
	CPasswordChunk chunk = CPasswordChunk(_nextChunk);

	std::string ret = chunk.GetPasswordBegin(),
		currentEnd = _nextChunk.GetPasswordEnd(),
		newBegin = getBeginFromEnd(currentEnd);

	// ... puis on en genere un autre pour le suivant
	generateChunk(newBegin);

	return chunk;
}

void OGlobal::StartServer(int port)
{
	// start a thread
	_server.Run(port);
}

void* OGlobal::ThreadKeyboardFunc(void *p_arg)
{
	bool* pAppRunning = reinterpret_cast<bool*>(p_arg);

	while (*pAppRunning)
	{
		char letter = _getch();

		if (letter == 27) // 'ESC'
			break;
	}

	std::cout << "KeyboardThread ending !" << std::endl;
	*pAppRunning = false;

	return nullptr;
}

void * OGlobal::ThreadServerFunc(void * p_arg)
{
	TcpServer *pServer = reinterpret_cast<TcpServer*>(p_arg);
	int port = PORT_TCP;

	pServer->Run(port);
	std::cout << "ServerThread ending !" << std::endl;

	return nullptr;
}

void OGlobal::StartKeyboardThread(const bool isBlocking)
{
	std::cout << "** Creating keyboard thread..." << std::endl;

	if (pthread_create(&_keyboardThread, nullptr, ThreadKeyboardFunc, reinterpret_cast<void*>(&_appRunning)) != 0) {
		std::cerr << "** FAIL keyboard thread" << std::endl;
		//return;// 1;
		exit(1);
	}

	Utils::mySleep(1000);

	if (isBlocking)
	{
		/* example */
		//while (this->_appRunning)
		//{
		//	std::cout << "RUNNING - ";
		//	Utils::mySleep(500);
		//}
		/* end example */

		/* example1 */
		void *result;
		std::cout << "** Waiting..." << std::endl;
		pthread_join(_keyboardThread, &result);
		/* end example1 */
	}
	//std::cout << std::endl << "KeyboardThread ended." << std::endl;
}

void OGlobal::StartServerThread()
{
	std::cout << "** Creating server thread..." << std::endl;

	if (pthread_create(&_serverThread, nullptr, ThreadServerFunc, reinterpret_cast<TcpServer*>(&_server)) != 0)
	{
		std::cerr << "** FAIL server thread" << std::endl;
		//return;// 1;
		exit(1);
	}
}

void OGlobal::Run()
{
	StartKeyboardThread(false);
	Utils::mySleep(1000);
	StartServerThread();

	/* example1 */
	void *result;
	std::cout << "** Waiting for keyboard escape..." << std::endl;
	pthread_join(_keyboardThread, &result);
	/* end example1 */

	_server.StopServer();

	std::cout << "Terminated." << std::endl;
}

/*
* @brief: Generates the next chunk, stores it and return its end
*/
std::string OGlobal::generateChunk(const std::string begin)
{
	static char lastInAlphabet = _alphabet[_alphabet.length() - 1];

	std::string end = begin;
	int lastIndex;
	int beginLength = begin.length();

	if (beginLength == 0)
		return "";

	lastIndex = beginLength - 1;

	for (int i = lastIndex; i > lastIndex - _chunkSize; --i)
	{
		end[i] = lastInAlphabet;
	}

	std::string logMsg = "[OGlobal] Generated : " + begin + " --> " + end;
	std::cout << logMsg << std::endl;
	_logger->LogInfo(0, logMsg);

	_nextChunk.Reset();
	_nextChunk.SetPasswordRange(begin, end);

	return end;
}

std::string OGlobal::getBeginFromEnd(std::string end)
{
	static char lastInAlphabet = _alphabet[_alphabet.length() - 1];
	int lastIndex;
	//std::string begin = end;
	char cbegin[64];
	int size = sizeof(cbegin);

	for (int i = 0; i < size; ++i)
	{
		if (i < end.length())
			cbegin[i] = end[i];
		else
			cbegin[i] = '\0';
	}

	HashCrackerUtils::IncreasePassword(cbegin, sizeof(cbegin), _alphabet);
	//int endLength = end.length();

	//if (endLength == 0)
	//	return "";

	//lastIndex = endLength - 1;

	//int i = lastIndex;
	//while (begin[i] == lastInAlphabet)
	//{
	//	begin.at(i) = _alphabet[0];

	//	if (--i == -1) // attention on decrement avant de tester
	//	{
	//		// ici use increasePass
	//		std::cout << "Nothing after " << end << std::endl;
	//		return "";
	//	}
	//}
	//begin.at(i) = _alphabet[IndexOf(begin[i]) + 1];
	//std::cout << "Begin after " << end << " is " << begin << std::endl;

	// TEST


	// END TEST

	return cbegin;
}

int OGlobal::IndexOf(const char letter) const
{
	int length = _alphabet.length();

	for (int i = 0; i < length; ++i)
	{
		if (_alphabet[i] == letter)
			return i;
	}

	return -1;
}

unsigned int OGlobal::GetChunkSize() const
{
	return _chunkSize;
}

void OGlobal::initNextChunk()
{
	const int nbPassLetters = _hash.length();
	std::string first = "";

	for (int i = 0; i < _chunkSize; ++i)
		first += _alphabet[0];

	generateChunk(first);
}
