#include "Agent.h"
#include "utils.h"
#include "PasswordChunk.h"
#include "OrdonnanceurLocal.h"
#include <math.h>
#include <CHashSha256.h>
#include <CHashMd5.h>

void* Agent::GenerationPassword(void *p_arg)
{
	Agent::paramThread *strucarg;
	try {
		strucarg = reinterpret_cast<struct Agent::paramThread*>(p_arg);// (struct Agent::paramThread*)p_arg;
	}
	catch (std::bad_alloc)
	{
		std::cout << "Erreur";
	}
	OrdonnanceurLocal *instanceol = strucarg->instance;
	std::string passwordtofind = strucarg->passwordToFind;
	bool isStop = strucarg->arret;
	bool trouve = false;
	while (!isStop&&trouve==false)
	{

		std::string lstPassword;
		
		CPasswordChunk chunkToGenerate= instanceol->GetChunk();
		std::string testAlphabet = "abcdefghijklomnpqrstuvwyz";
		 double sizeOFChunk = pow(chunkToGenerate.GetChunkSize(),sizeof(testAlphabet));
			char password[64] = "";

		strcpy_s(password, sizeof(password),chunkToGenerate.GetPasswordBegin().c_str());
		
		while (password!=chunkToGenerate.GetPasswordEnd()&&trouve==false)
		{
			HashCrackerUtils::IncreasePassword(password, sizeof(password), testAlphabet);
			if (password == passwordtofind)
			{
				trouve = true;
			}
			std::cout << password <<std::endl;
		}
	}
	return nullptr;
}

bool Agent::ComparerPassword(std::string passwordToTest, std::string passwordToDiscover)
{
	return passwordToTest == passwordToDiscover;
}
