#pragma once
#include "PasswordChunk.h"
#include "OrdonnanceurLocal.h"
static class Agent
{
public:

	struct paramThread
	{
		OrdonnanceurLocal* instance;
		std::string passwordToFind;
		bool arret;
	};

	//Méthode permettant de générer les password (point d'entrée des threads)
	static void* GenerationPassword(void *p_arg);

	//Méthode pour obtenir de getter le chunk de la fifo
	//Comparaison password à découvrir et celui généré
	static bool ComparerPassword(std::string passwordToTest, std::string passwordToDiscover);
};

