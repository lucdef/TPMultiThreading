#include "OGlobalTest.hpp"
#include "OGlobal.hpp"


OGlobalTest::OGlobalTest()
{
}


OGlobalTest::~OGlobalTest()
{
}

int OGlobalTest::TestGetBeginFromEnd(std::string dico, int chunkSize, int passLetters)
{
	OGlobal og = OGlobal(1, chunkSize, "", dico);
	std::string first = "", last = "";

	// on rempli le pass de fin avec un 9 a la fin
	for (int i = 0; i < passLetters - 2; ++i)
		last += dico[0];
	last += dico[dico.length() - 1];

	for (int i = passLetters - 2; i >= 0; --i)
	{
		// on ajoute un "dernierChar" de plus a chaque fois (ex: pour dico="0123" on ajoute des '3')
		last.at(i) = dico.at(dico.length() - 1);
		first = og.getBeginFromEnd(last);

		//if (first.length() == 0)
		//	break;
	}


	return 0;
}

int OGlobalTest::TestGenerateChunk(std::string dico, int chunkSize, int passLetters)
{
	OGlobal og = OGlobal(1, chunkSize, "", dico);
	std::string first = "";

	//double maxPass = std::pow(passLetters - chunkSize, dico.length() ) - 50;


	TestGetBeginFromEnd(dico, chunkSize, passLetters);

	for (int i = 0; i < passLetters; ++i)
	{
		first += dico[0];
	}

	std::string last = og.generateChunk(first);

	int i = 0;
	while (last.length() != 0)
	{
		if (i == 1000)
			std::cout << "1000" << std::endl;

		first = og.getBeginFromEnd(last);
		last = og.generateChunk(first);
		//std::cerr << "count: " << i << std::endl;

		++i;
	}
	std::cout << "Finished." << std::endl;

	return 0;
}

int OGlobalTest::TestMain()
{
	std::string dico = "01234abcef*";
	short chunkSize = 2;
	int passLetters = 5;

	TestGetBeginFromEnd(dico, chunkSize, passLetters);
	TestGenerateChunk(dico, chunkSize, passLetters);

	return 0;
}