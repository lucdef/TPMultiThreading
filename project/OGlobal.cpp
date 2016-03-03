#include "OGlobal.hpp"
#include "utils.h"
#include "conio.h"

OGlobal* OGlobal::_instance = nullptr;



OGlobal::OGlobal(const int nbThread, const int chunkSize, const std::string algo, const std::string hash, const std::string alphabet)
	: _server(),
	_algo(algo),
	_appRunning(true),
	_hash(hash),
	_alphabet(alphabet),
	_chunkSize(chunkSize)
{
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
	if(_instance == nullptr && nbThread != -1)
		_instance = new OGlobal(nbThread, chunkSize, algo, hash, alphabet);

	return _instance;
}

void OGlobal::Kill()
{
	delete(_instance);
	_instance = nullptr;
}

const std::string OGlobal::GetAlphabet() const
{
	return _alphabet;
}

const std::string OGlobal::GetAlgo() const
{
	return _algo;
}

const std::string OGlobal::GetHash() const
{
	return _hash;
}

const std::string OGlobal::GetNextChunkBegin() const
{
	return _nextChunk.GetPasswordBegin();
}

const std::string OGlobal::CraftResponse(const std::string request)
{
	return "";
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
	int port = 666;
	
	
	pServer->Run(port);

	std::cout << "ServerThread ending !" << std::endl;

	return nullptr;
}

void OGlobal::StartKeyboardThread(const bool isBlocking)
{
	//_keyboardArgs.appRunning = false;

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
	if (pthread_create(&_serverThread, nullptr, ThreadServerFunc, reinterpret_cast<TcpServer*>(&_server)) != 0) {
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

	
	/* example */
	//while (this->_appRunning)
	//{
	//	std::cout << "RUNNING - ";
	//	Utils::mySleep(1000);
	//}
	/* end example */

	/* example1 */
	void *result;
	std::cout << "** Waiting for keyboard escape..." << std::endl;
	pthread_join(_keyboardThread, &result);
	/* end example1 */

	_server.StopServer();


	std::cout << "Terminated." << std::endl;
}

std::string OGlobal::generateChunk(const std::string begin)
{
	static char lastInAlphabet = _alphabet[_alphabet.length() -1];

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

	std::cout << "generated : " << begin << " --> " << end << std::endl ;
	//_nextChunk = 

	_nextChunk.Reset();
	_nextChunk.SetPasswordRange(begin, end);

	return end;
}

std::string OGlobal::getBeginFromEnd(const std::string end)
{
	static char lastInAlphabet = _alphabet[_alphabet.length() - 1];
	int lastIndex;
	std::string begin = end;
	int endLength = end.length();

	if (endLength == 0)
		return "";

	lastIndex = endLength - 1;


	int i = lastIndex;
	while (begin[i] == lastInAlphabet)
	{
		begin.at(i) = _alphabet[0];

		if (--i == -1) // attention on decrement avant de tester
		{
			std::cout << "Nothing after " << end << std::endl;
			return "";
		}
	}
	begin.at(i) = _alphabet[IndexOf(begin[i]) + 1];

	std::cout << "Begin after " << end << " is " << begin << std::endl;


	return begin;
}

int OGlobal::IndexOf(char letter)
{
	int length = _alphabet.length();
	
	for (int i = 0; i < length; ++i)
	{
		if (_alphabet[i] == letter)
			return i;
	}

	return -1;
}



//void OGlobal::replyChunk()
//{
//	std::string trame = "NEW-CHUNK-FOR-YOU=" + _nextChunk.GetPasswordBegin();
//	_server.SendData(trame); // TODO: handle several clients
//}

//void OGlobal::createThreads()
//{
//
//}
