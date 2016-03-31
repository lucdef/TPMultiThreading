#include "OGlobalTest.hpp"
#include "OGlobal.hpp"

OGlobalTest::OGlobalTest()
{
}

OGlobalTest::~OGlobalTest()
{
}

//int OGlobalTest::TestGetBeginFromEnd(std::string dico, int chunkSize, int passLetters)
//{
//	OGlobal *og = OGlobal::GetInstance(1, chunkSize, "sha256", "XXXXXXXXXX", dico);
//	std::string first = "", last = "";
//
//	// on rempli le pass de fin avec un 9 a la fin
//	for (int i = 0; i < passLetters - 2; ++i)
//		last += dico[0];
//	last += dico[dico.length() - 1];
//
//	for (int i = passLetters - 2; i >= 0; --i)
//	{
//		// on ajoute un "dernierChar" de plus a chaque fois (ex: pour dico="0123" on ajoute des '3')
//		last.at(i) = dico.at(dico.length() - 1);
//		first = og->getBeginFromEnd(last);
//	}
//
//	OGlobal::Kill();
//	return 0;
//}

//int OGlobalTest::TestKeyboardThread(std::string dico, int chunkSize, int passLetters)
//{
//	OGlobal *og = OGlobal::GetInstance(1, chunkSize, "sha256", "XXXXXXXXXX", dico);
//	og->StartKeyboardThread(true);
//
//	return 0;
//}
//
//int OGlobalTest::TestServerThread(std::string dico, int chunkSize, int passLetters)
//{
//	OGlobal *og = OGlobal::GetInstance(1, chunkSize, "sha256", "XXXXXXXXXX", dico);
//	og->StartServerThread();
//
//	return 0;
//}

int OGlobalTest::TestServerAndKeyboard(std::string dico, int chunkSize, int passLetters)
{
	chunkSize = 1;
	OGlobal *og = OGlobal::GetInstance(1, chunkSize, "sha256", "f284bdc3c1c9e24a494e285cb387c69510f28de51c15bb93179d9c7f28705398", dico);
	og->Run();

	return 0;
}

int OGlobalTest::TestGenerateChunk(std::string dico, int chunkSize, int passLetters)
{
	//TestGetBeginFromEnd(dico, chunkSize, passLetters);
	chunkSize = 3;
	OGlobal *og = OGlobal::GetInstance(1, chunkSize, "sha256", "XXXXXXXXXX", dico);
	std::string first = "";

	//for (int i = 0; i < passLetters; ++i)
	for (int i = 0; i < chunkSize; ++i)
	{
		first += dico[0];
	}

	//std::cout << pass << std::endl;
	std::string last;
	for (int i = 0; i < 10; ++i)
	{
		last = og->GetNextChunk().GetPasswordEnd();
	}
	// END TEST


	return 0;
}

int OGlobalTest::TestMain()
{
	std::string dico = "01234abcef*";
	short chunkSize = 2;
	int passLetters = 5;

	//TestGetBeginFromEnd(dico, chunkSize, passLetters);
	TestGenerateChunk(dico, chunkSize, passLetters);
	//TestKeyboardThread(dico, chunkSize, passLetters);

	return 0;
}