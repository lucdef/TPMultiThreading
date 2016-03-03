#pragma once
#include <string>

class OGlobalTest
{
public:
	OGlobalTest();
	~OGlobalTest();
	static int TestMain();
	static int TestGenerateChunk(std::string dico = "01234abcef*", int chunkSize = 2, int passLetters = 5);
	static int TestGetBeginFromEnd(std::string dico = "01234abcef*", int chunkSize = 2, int passLetters = 5);
	static int TestKeyboardThread(std::string dico = "01234abcef*", int chunkSize = 2, int passLetters = 5);
	static int TestServerThread(std::string dico = "01234abcef*", int chunkSize = 2, int passLetters = 5);
	static int TestServerAndKeyboard(std::string dico = "01234abcef*", int chunkSize = 2, int passLetters = 5);
};

