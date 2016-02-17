#pragma once
#include "PasswordChunk.h"
#include "OrdonnanceurLocal.h"
static class Agent
{
public:
	//Méthode permettant de générer les password (point d'entrée des threads)
	static void* GenerationPassword(void *p_arg);
	//Méthode pour obtenir de getter le chunk de la fifo
	static CPasswordChunk GetChunk();
	//Comparaison password à découvrir et celui généré
	static bool ComparerPassword(std::string passwordToTest, std::string passwordToDiscover);
	//Hashera le password 
	static std::string SHA256Hash(std::string passwordToHash);
	struct paramThread
	{
		OrdonnanceurLocal *instance;
		std::string passwordToFind;
		bool arret;
	};
	
};

