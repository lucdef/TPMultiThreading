#pragma once

#include <iostream>


namespace HashCrackerUtils {
	void ParseCommandLine(const int p_argc, const char *p_argv[], std::string &p_hash, std::string &p_algo, std::string &p_alphabet, unsigned int &p_chunkSize, std::string &p_masterIpAddress, bool &p_slaveMode);
	void IncreasePassword(char *p_password, const unsigned int p_bufferCapacity, const std::string &p_alphabet);
}

class Utils
{
public:
	virtual ~Utils() = 0;
	static void mySleep(int sleepMs);
	static bool StringContains(const std::string & source, const std::string & pattern);
};