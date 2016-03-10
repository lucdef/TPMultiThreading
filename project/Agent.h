#pragma once
#include "PasswordChunk.h"
#include "OrdonnanceurLocal.h"
static class Agent
{
public:
	//M�thode permettant de g�n�rer les password (point d'entr�e des threads)
	static void* GenerationPassword(void *p_arg);
	//M�thode pour obtenir de getter le chunk de la fifo
	static CPasswordChunk GetChunk();
	//Comparaison password � d�couvrir et celui g�n�r�
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

