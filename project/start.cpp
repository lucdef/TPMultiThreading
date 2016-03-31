#include "stdafx.h"
#include <iostream>
#include <deque>
#include "FIFO.h"
#include "utils.h"
#include "CException.h"
#include "PasswordChunk.h"
#include "OrdonnanceurLocal.h"
#include "HashUtils.h"

#include <pthread.h>
#include <Windows.h>
#include "LogManager.h"

#include "TcpTest.hpp"
#include "OGlobalTest.hpp"

int main( int argc, const char *argv[] ) {
	
	unsigned int chunkSize, passLetter = 5; // TODO passLetter??
	std::string hash,
		algo,
		alphabet,
		masterIpAddress;

	try
	{
		Utils::ParseCommandLine(argc,
			argv,
			hash,
			algo,
			alphabet,
			chunkSize,
			masterIpAddress);
	}
	catch(CException& cex)
	{
		std::cout << "\nErreur d'arguments...\nAppuyer sur <Enter> pour continuer";
		std::cin.get();
		return EXIT_FAILURE;
	}

	
	if (masterIpAddress.length() != 0)
	{

		OrdonnanceurLocal ordo(masterIpAddress);
		ordo.StartThread();
		ordo.WaitThreads();
		HashUtils::GetInstance()->Kill();
		//// ici ca se detruit parce que StartThread n'est pas une boucle infini,
		// donc des que ca retourne, on sort du if (ici) et du coup on détruit l'ordo
		// fix: faire une methode run en boucle 'infinie' (sur un bool par exemple)
	}
	else
	{
		/* GO GLOBAL HERE */

	//int res = TcpTest::TestMain();
	int res = OGlobalTest::TestServerAndKeyboard();
	//int res = OGlobalTest::TestGenerateChunk();
	//int res = OGlobalTest::TestKeyboardThread();
	//int res = OGlobalTest::TestServerThread();
	}


	std::cout << "\nAppuyer sur <Enter> pour continuer";
	std::cin.get();
	LogManager::GetInstance()->Kill();
	
	return EXIT_SUCCESS;
}
