#include "stdafx.h"
#include <iostream>
#include "utils.h"
#include "CException.h"
#include "OrdonnanceurLocal.h"
#include "HashUtils.h"
#include "LogManager.h"
#include "OGlobal.hpp"

int main( int argc, const char *argv[] )
{	
	unsigned int chunkSize, passLetter = 5;
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
		ordo.Run();
		
		HashUtils::GetInstance()->Kill();
	}
	else
	{
		if(alphabet.length() == 0)
		{
			std::cout << "Veuillez saisir un alphabet." << std::endl;
			return EXIT_SUCCESS;
		}

		OGlobal *og = OGlobal::GetInstance(1, chunkSize, algo, "f284bdc3c1c9e24a494e285cb387c69510f28de51c15bb93179d9c7f28705398", alphabet);
		//OGlobal *og = OGlobal::GetInstance(1, chunkSize, algo, hash, alphabet);
		og->Run();

		// TODO: kill og...
		//og->Kill();
	}

	LogManager::GetInstance()->Kill();


	std::cout << "\nAppuyer sur <Enter> pour continuer";
	std::cin.get();
	return EXIT_SUCCESS;
}
