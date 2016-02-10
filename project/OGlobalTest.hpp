#pragma once
#include <string>

class OGlobalTest
{
	public:
		OGlobalTest();
		~OGlobalTest();
		static int TestMain();
		static int TestGenerateChunk(std::string dico, int chunkSize, int passLetters);
		static int TestGetBeginFromEnd(std::string dico, int chunkSize, int passLetters);
};

