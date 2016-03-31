#include "stdlib.h"
#include <chrono>
#include <thread>
#include "Agent.h"
#include "utils.h"
#include "LogManager.h"
#include "PasswordChunk.h"
#include "OrdonnanceurLocal.h"
#include <sstream>
#include <cctype>


void* Agent::GenerationPassword(void *p_arg)
{
	OrdonnanceurLocal::paramThread *strucarg;
	strucarg = reinterpret_cast<struct OrdonnanceurLocal::paramThread*>(p_arg);// (struct Agent::paramThread*)p_arg;
	OrdonnanceurLocal *instanceol = strucarg->instance;
	std::string passwordtofind = strucarg->passwordToFind;
	void* passwordFind = "";
	bool trouve = false;
	std::string alphabet;
	int sizeOFChunk = 0;

	//On boucle tant qu'on n'a pas trouver le password ou que on a pas reçu une commande stop
	while (!strucarg->arret)
	{
		if (!trouve) {
			CPasswordChunk chunkToGenerate = instanceol->GetChunk();
			if (chunkToGenerate.GetPasswordBegin() == "f*0")
			{
				std::cout << "test";
}
			sizeOFChunk = chunkToGenerate.GetChunkSize();
			char password[64] = "";

			strcpy_s(password, sizeof(password), chunkToGenerate.GetPasswordBegin().c_str());
			alphabet = instanceol->GetAlphabet();
			/*bool test = password <= chunkToGenerate.GetPasswordEnd();*/
			std::string spassword(password);
			int test = spassword.compare( chunkToGenerate.GetPasswordEnd());
			int nbtest = chunkToGenerate.GetChunkSize();
			int nb = pow(alphabet.length(), chunkToGenerate.GetChunkSize());
			int i = 0;
			while (i<=nb && trouve == false)
			{
				//Si on trouve le password on quitte les autres threads
				std::string passwordHash = instanceol->HashPassword(password);
				std::transform(passwordHash.begin(), passwordHash.end(), passwordHash.begin(), ::tolower);
				if (passwordtofind == passwordHash)
				{
					strucarg->instance->SetPasswordFind(password);
					trouve = true;
					instanceol->FoundPassword(password);
					std::ostringstream message;
					message << "Password found :" << password << " hashed password:" << passwordtofind;
					LogManager::GetInstance()->LogInfo(0, message.str());
				}

				std::ostringstream message;
				message << "Password: " << password << " passwordHashed: " << passwordHash;
				LogManager::GetInstance()->LogInfo(1, message.str());
				HashCrackerUtils::IncreasePassword(password, sizeof(password), alphabet);

				i++;
			}
		}
	}
	
	return nullptr;
}
