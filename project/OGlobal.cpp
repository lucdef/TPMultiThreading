#include "OGlobal.hpp"
#include "utils.h"
#include "conio.h"
#include "CSocket.h"

OGlobal* OGlobal::_instance = nullptr;

OGlobal::OGlobal(const int nbThread, const int chunkSize, const std::string algo, const std::string hash, const std::string alphabet)
	: _server(),
	_algo(algo),
	_appRunning(true),
	_hash(hash),
	_alphabet(alphabet),
	_chunkSize(chunkSize),
	_totalChunkSize(-1),
	_nextChunk()
{
	_logger = LogManager::GetInstance();
	_totalChunkSize = pow(_alphabet.length(), _chunkSize);
	_currentPassSize = 1;

	// init next chunk
	initNextChunk();
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
	// celui stocké et le bon...
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
		void *result;
		std::cout << "** Waiting..." << std::endl;
		pthread_join(_keyboardThread, &result);
	}
}

void OGlobal::StartServerThread()
{
	std::cout << "** Creating server thread..." << std::endl;

	if (pthread_create(&_serverThread, nullptr, ThreadServerFunc, reinterpret_cast<TcpServer*>(&_server)) != 0)
	{
		std::cerr << "** FAIL server thread" << std::endl;
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

std::string OGlobal::generateChunk(const std::string begin) const
{
	static char lastInAlphabet = _alphabet[_alphabet.length() - 1];
	std::string end = "";
	static const int dicoLength = _alphabet.length();
	int i = 0;
	int dicoCpt = i;

	end += _alphabet[0];
	for (; i < _totalChunkSize; ++i, ++dicoCpt )
	{
		if(dicoCpt == dicoLength)
		{
			int length = end.length();
			if(length < _currentPassSize)
			{
				end = _alphabet[0] + end;
			}

			dicoCpt = 0;
		}

		end[end.length() - 1] = _alphabet[dicoCpt];

	}


	return end;
}

std::string OGlobal::getBeginFromEnd(const std::string end) const
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

void OGlobal::initNextChunk() const
{
	const int nbPassLetters = _hash.length();
	std::string first = "" + _alphabet[0];
	std::string end = "";

	std::cout << _totalChunkSize;

	generateChunk(first);
}
