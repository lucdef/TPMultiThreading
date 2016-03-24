#include "HashUtils.h"


HashUtils* HashUtils::_instance;


HashUtils* HashUtils::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new HashUtils();
	}
	return _instance;
}

std::string HashUtils::hashMd5(std::string password)
{
	this->_chashShaMD5->HashBuffer(reinterpret_cast<const unsigned char *>(password.c_str()), static_cast<int>(password.length()));
	return this->_chashShaMD5->GetHash();
}

std::string HashUtils::HashSha224(std::string password)
{
	this->_chashSha224->HashBuffer(reinterpret_cast<const unsigned char *>(password.c_str()), static_cast<int>(password.length()));
	return this->_chashSha224->GetHash();
}

std::string HashUtils::HashSha256(std::string password)
{
	this->_chashSha256->HashBuffer(reinterpret_cast<const unsigned char *>(password.c_str()), static_cast<int>(password.length()));
	return this->_chashSha256->GetHash();
}

std::string HashUtils::HashSha1(std::string password)
{
	this->_chashSha1->HashBuffer(reinterpret_cast<const unsigned char *>(password.c_str()), static_cast<int>(password.length()));
	return this->_chashSha1->GetHash();
}

std::string HashUtils::HashCrc32(std::string password)
{
	this->_chashCrc32->HashBuffer(reinterpret_cast<const unsigned char *>(password.c_str()), static_cast<int>(password.length()));
	return this->_chashCrc32->GetHash();
}

void HashUtils::Kil()
{
	if (HashUtils::_instance != nullptr)
	{
		HashUtils::_instance->~HashUtils();
	}
}

HashUtils::HashUtils()
{
	this->_chashCrc32 = new CHashCrc32();
	this->_chashSha1 = new CHashSha1();

	this->_chashShaMD5 = new CHashMd5();
	this->_chashSha224 = new CHashSha224();
	this->_chashSha256 = new CHashSha256();
}


HashUtils::~HashUtils()
{
	delete this->_chashCrc32;
	delete this->_chashShaMD5;
	delete this->_chashSha256;
	delete this->_chashSha224;
	delete this->_chashSha1;
}
