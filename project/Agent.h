#pragma once
#include "PasswordChunk.h"
#include "OrdonnanceurLocal.h"
static class Agent
{
public:
	//Méthode permettant de générer les password (point d'entrée des threads)
	static void* GenerationPassword(void* p_args);
	//Méthode pour obtenir de getter le chunk de la fifo
	//Comparaison password à découvrir et celui généré
	static bool ComparerPassword(std::string passwordToTest, std::string passwordToDiscover);
	//Hashera le password 
	static std::string SHA256Hash(std::string passwordToHash);
	
};

