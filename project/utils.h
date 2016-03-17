#pragma once

#include <iostream>
#include "CSocket.h"

namespace HashCrackerUtils
{
	void ParseCommandLine(const int p_argc,
							const char *p_argv[],
							std::string &p_hash,
							std::string &p_algo,
							std::string &p_alphabet,
							unsigned int &p_chunkSize,
							std::string &p_masterIpAddress,
							std::string &ordolocal,
							std::string &ordoGlobal);
	
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
};