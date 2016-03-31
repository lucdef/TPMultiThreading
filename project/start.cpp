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

//void ExtractCommandLine(int argc, const char *argv[]) {
//	// Command line settings
//	// Tip: good to be stored in a singleton dedicated to configuration	--> see singleton.h
//	std::string hash, algo, alphabet, masterIpAddress;
//	unsigned int chunkSize = 0;
//	bool runningAsSlave = false;
//}

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
	
	//OrdonnanceurLocal ordolocal;

	//ExtractCommandLine( argc, argv, ordolocal.getCommandLine());

	//ordolocal.StartThread();

	//std::cout << std::endl;
	//std::cout << "** Goodbye" << std::endl;
	//std::cin.get();
	//return EXIT_SUCCESS;

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
	HashUtils::GetInstance()->Kill();
	return EXIT_SUCCESS;
}
