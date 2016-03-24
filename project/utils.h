#pragma once

#include <iostream>
#include "CSocket.h"

namespace HashCrackerUtils
{

	
	void IncreasePassword(char *p_password, 
							const unsigned int p_bufferCapacity, 
							const std::string &p_alphabet);
}

class Utils
{
public:
	virtual ~Utils() = 0;
	static void mySleep(int sleepMs);
	static bool StringContains(const std::string & source, const std::string & pattern);
	static std::string GetClientStr(CSocket* const client);
	static std::string GetPatternFromData(const std::string & haystack, const std::string & needle);
	static void ParseCommandLine(const int p_argc,
		const char *p_argv[],
		std::string &p_hash,
		std::string &p_algo,
		std::string &p_alphabet,
		unsigned int &p_chunkSize,
		std::string &p_masterIpAddress);
	static const std::string _patternAlgo;
	static const std::string _patternHash;
	static const std::string _patternAlphabet;
	static const std::string _patternChunk;

};