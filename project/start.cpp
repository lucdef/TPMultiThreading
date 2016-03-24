#include "stdafx.h"
#include <iostream>
#include <deque>
#include "FIFO.h"
#include "utils.h"
#include "CException.h"
#include "PasswordChunk.h"
#include "OrdonnanceurLocal.h"

#include <pthread.h>
#include <Windows.h>
#include "LogManager.h"

#include "TcpTest.hpp"
#include "OGlobalTest.hpp"

void ExtractCommandLine(int argc, const char *argv[]) {
	// Command line settings
	// Tip: good to be stored in a singleton dedicated to configuration	--> see singleton.h
	std::string hash, algo, alphabet, masterIpAddress;
	unsigned int chunkSize = 0;
	bool runningAsSlave = false;
}

//void ExtractCommandLine( int argc, 
//						const char *argv[] , 
//						OrdonnanceurLocal::strCommandLine* stCommand)
//{
//	// Extract command line
//	try {
//		HashCrackerUtils::ParseCommandLine(argc, 
//											argv, 
//											stCommand->hash, 
//											stCommand->algo,
//											stCommand->alphabet,
//											stCommand->chunkSize,
//											stCommand->masterIPAddress,
//											stCommand->OrdoLocal,
//											stCommand->OrdoGlobal);
//	}
//	catch(CException &ex) {
//		std::cerr << "** Command line extraction failed at \"" << ex.GetFaultLocation() << "\" with error code " << ex.GetErrorCode() << " and message \"" << ex.GetErrorMessage() << "\"" << std::endl;
//		return;
//	}
//
//	// Show information
//	std::cout << "--- INFORMATION GOT FROM COMMAND LINE ---" << std::endl;
//
//	if (stCommand->OrdoLocal == "NO" && stCommand->OrdoGlobal == "YES")
//		std::cout << "ordoGlobal only" << std::endl;
//	else if (stCommand->OrdoLocal == "YES" && stCommand->OrdoGlobal == "YES")
//		std::cout << "both ordoLocal and ordoGlobal" << std::endl;
//	else if (stCommand->OrdoLocal == "YES" && stCommand->OrdoGlobal == "NO")
//		std::cout << "ordoLocal only" << std::endl;
//
//	std::cout << "-ip " << stCommand->masterIPAddress << std::endl;
//	std::cout << "-hash " << stCommand->hash << std::endl;
//	std::cout << "-algo " << stCommand->algo << std::endl;
//	std::cout << "-alphabet " << stCommand->alphabet << std::endl;
//	std::cout << "-chunksize " << stCommand->chunkSize << std::endl;
//	std::cout << "-OrdoLocal " << stCommand->OrdoLocal << std::endl;
//	std::cout << "-OrdoGlobal " << stCommand->OrdoGlobal << std::endl;
//}

int main( int argc, const char *argv[] ) {

	LogManager::GetInstance()->LogError(1, "ta mère en slip");

		std::string  p_hash;
		std::string  p_algo;
		std::string  p_alphabet;
		unsigned int  p_chunkSize;
		std::string  p_masterIpAddress;
		std::string  ordolocal;
		std::string  ordoGlobal;

	Utils::ParseCommandLine( argc,
			argv,
			p_hash,
			p_algo,
			p_alphabet,
			p_chunkSize,
			p_masterIpAddress,
			ordolocal,
			ordoGlobal);
	if (ordolocal == "YES")
	{
		OrdonnanceurLocal *ordo = new OrdonnanceurLocal("127.0.0.1");
		std::cout << "nop";
		delete ordo;
	}
	//OrdonnanceurLocal ordolocal;

	//ExtractCommandLine( argc, argv, ordolocal.getCommandLine());

	//ordolocal.StartThread();

	//std::cout << std::endl;
	//std::cout << "** Goodbye" << std::endl;
	//std::cin.get();
	//return EXIT_SUCCESS;

	
	//int res = TcpTest::TestMain();
	/*int res = OGlobalTest::TestServerAndKeyboard();*/
	//int res = OGlobalTest::TestKeyboardThread();
	//int res = OGlobalTest::TestServerThread();


	std::cout << "\nAppuyer sur <Enter> pour continuer";
	std::cin.get();
	return EXIT_SUCCESS;
}
