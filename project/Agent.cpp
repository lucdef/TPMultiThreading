#include "Agent.h"
#include "utils.h"
#include "PasswordChunk.h"
#include "OrdonnanceurLocal.h"

void* Agent::GenerationPassword(void *p_arg)
{
	OrdonnanceurLocal::paramThread *strucarg;
	try {
		strucarg = reinterpret_cast<struct OrdonnanceurLocal::paramThread*>(p_arg);// (struct Agent::paramThread*)p_arg;
	}
	catch (std::bad_alloc)
	{
		std::cout << "Erreur";
	}
	OrdonnanceurLocal *instanceol = strucarg->instance;
	std::string passwordtofind = strucarg->passwordToFind;
	void* passwordFind = "";
	bool trouve = false;
	
	while (!strucarg->arret)
	{

		CPasswordChunk chunkToGenerate= instanceol->GetChunk();
		int sizeOFChunk = chunkToGenerate.GetChunkSize();
			char password[64] = "";

		strcpy_s(password, sizeof(password),chunkToGenerate.GetPasswordBegin().c_str());
		std::string testAlphabet = "abcdefhijklomnpqrstuvwyz";
		while(password<=chunkToGenerate.GetPasswordEnd()&&trouve==false)
		{
			if (passwordtofind == password)
			{

				trouve = true;
				strucarg->arret = true;
				instanceol->StopThread();
				passwordFind = password;


			}
			HashCrackerUtils::IncreasePassword(password, sizeof(password), testAlphabet);
			
			std::cout << password <<std::endl;
		}
	}
	
	return passwordFind;
}
