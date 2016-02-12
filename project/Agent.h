#pragma once
#include "PasswordChunk.h"
static class Agent
{
public:
	//Méthode permettant de générer les password (point d'entrée des threads)
	static void GenerationPassword(CPasswordChunk chunkToGenerate);
	//Méthode pour obtenir de getter le chunk de la fifo
	static CPasswordChunk GetChunk();
	//Comparaison password à découvrir et celui généré
	static bool ComparerPassword(std::string passwordToTest, std::string passwordToDiscover);
	//Hashera le password 
	static std::string SHA256Hash(std::string passwordToHash);
	
};

