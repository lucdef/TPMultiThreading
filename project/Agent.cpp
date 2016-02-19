#include "Agent.h"
#include "utils.h"
#include "PasswordChunk.h"
#include "OrdonnanceurLocal.h"


void* Agent::GenerationPassword(void *p_arg)
{
	OrdonnanceurLocal::paramThread *strucarg;
	strucarg = reinterpret_cast<struct OrdonnanceurLocal::paramThread*>(p_arg);// (struct Agent::paramThread*)p_arg;
	OrdonnanceurLocal *instanceol = strucarg->instance;
	std::string passwordtofind = strucarg->passwordToFind;
	void* passwordFind = "";
	bool trouve = false;
	//On boucle tant qu'on n'a pas trouver le password ou que on � pas re�u une commande stop
	while (!strucarg->arret)
	{
		if (!trouve) {
			CPasswordChunk chunkToGenerate = instanceol->GetChunk();
			int sizeOFChunk = chunkToGenerate.GetChunkSize();
			char password[64] = "";

			strcpy_s(password, sizeof(password), chunkToGenerate.GetPasswordBegin().c_str());
			std::string testAlphabet = "abcdefhijklomnpqrstuvwyz";
			while (password <= chunkToGenerate.GetPasswordEnd() && trouve == false)
			{
				//Si on trouve le password on quitte les autres threads
				if (passwordtofind == password)
				{
					strucarg->instance->SetPasswordFind(password);
					trouve = true;
					instanceol->StopThread();
					
				}
				HashCrackerUtils::IncreasePassword(password, sizeof(password), testAlphabet);
				std::cout << password << std::endl;
			}
		}
	}
	
	return nullptr;
}
