#include "OGlobal.hpp"





OGlobal::OGlobal(const int nbThread, const int chunkSize, std::string algo, std::string alphabet)
	: _server(),
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

std::string OGlobal::generateChunk(std::string begin)
{
	static char lastInAlphabet = _alphabet[_alphabet.length() -1];

	std::string end = begin;
	int lastIndex = begin.length() - 1;

	for (int i = lastIndex; i > lastIndex - _chunkSize; --i)
	{
		end[i] = lastInAlphabet;
	}

	std::cout << "generated : " << begin << " --> " << end << std::endl ;
	//_nextChunk = 

	return end;
}

std::string OGlobal::getBeginFromEnd(const std::string end)
{
	static char lastInAlphabet = _alphabet[_alphabet.length() - 1];
	int lastIndex = end.length() - 1;
	std::string begin = end;

	int i = lastIndex;

	while (begin[i] == lastInAlphabet)
	{
		begin.at(i) = _alphabet[0];
		--i;
	}
	begin.at(i) = _alphabet[Index(begin[i]) + 1];

	//std::cout << "Begin after " << end << " is " << begin << std::endl;


	return begin;
}

int OGlobal::Index(char letter)
{
	int length = _alphabet.length();
	
	for (int i = 0; i < length; ++i)
	{
		if (_alphabet[i] == letter)
			return i;
	}

	return -1;
}

//void OGlobal::createThreads()
//{
//
//}
