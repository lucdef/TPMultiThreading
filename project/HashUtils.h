#pragma once
#include <string>
#include "CHashSha256.h"
#include "CHashSha224.h"
#include "CHashSha1.h"
#include "CHashMd5.h"
#include "CHashCrc32.h"
class HashUtils
{
public:
	static HashUtils* GetInstance();
	std::string hashMd5(std::string password);
	std::string HashSha224(std::string password);
	std::string HashSha256(std::string password);
	std::string HashSha1(std::string password);
	std::string HashCrc32(std::string password);
	static void Kil();

private:
	CHashCrc32 *_chashCrc32;
	CHashSha256 *_chashSha256;
	CHashSha1 *_chashSha1;
	CHashSha224 *_chashSha224;
	CHashMd5 *_chashShaMD5;
	static HashUtils* _instance;
	HashUtils();
	~HashUtils();
};

