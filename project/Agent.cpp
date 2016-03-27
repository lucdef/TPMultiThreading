#include "stdlib.h"
#include <chrono>
#include <thread>
#include "Agent.h"
#include "utils.h"
#include "LogManager.h"
#include "PasswordChunk.h"
#include "OrdonnanceurLocal.h"
#include "HashUtils.h"
#include <sstream>


void* Agent::GenerationPassword(void *p_arg)
{
	OrdonnanceurLocal::paramThread *strucarg;
	strucarg = reinterpret_cast<struct OrdonnanceurLocal::paramThread*>(p_arg);// (struct Agent::paramThread*)p_arg;
	OrdonnanceurLocal *instanceol = strucarg->instance;
	std::string passwordtofind = strucarg->passwordToFind;
	void* passwordFind = "";
	bool trouve = false;
	//On boucle tant qu'on n'a pas trouver le password ou que on à pas reçu une commande stop
	while (!strucarg->arret)
	{
		if (!trouve) {
			CPasswordChunk chunkToGenerate = instanceol->GetChunk();
			int sizeOFChunk = chunkToGenerate.GetChunkSize();
			char password[64] = "";

			strcpy_s(password, sizeof(password), chunkToGenerate.GetPasswordBegin().c_str());
			std::string alphabet = instanceol->GetAlphabet();
			while (password <= chunkToGenerate.GetPasswordEnd() && trouve == false)
			{
				//Si on trouve le password on quitte les autres threads
				if (passwordtofind == instanceol->HashPassword(password))
				{
					strucarg->instance->SetPasswordFind(password);
					trouve = true;
					instanceol->FoundPassword(password);
					std::ostringstream message;
					message << "Password found :" << password << " hashed password:" << passwordtofind;
					LogManager::GetInstance()->LogInfo(0, message.str());
				}
				//std::this_thread::sleep_for(std::chrono::milliseconds(80));
				HashCrackerUtils::IncreasePassword(password, sizeof(password), alphabet);
			}
		}
	}
	
	return nullptr;
}
