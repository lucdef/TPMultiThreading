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
	pthread_mutex_lock(&_mutex);
	this->_chashShaMD5->HashBuffer(reinterpret_cast<const unsigned char *>(password.c_str()), static_cast<int>(password.length()));
	std::string hash = this->_chashShaMD5->GetHash();
	pthread_mutex_unlock(&_mutex);
	return hash;
}

std::string HashUtils::HashSha224(std::string password)
{
	pthread_mutex_lock(&_mutex);
	this->_chashSha224->HashBuffer(reinterpret_cast<const unsigned char *>(password.c_str()), static_cast<int>(password.length()));
	std::string hash = this->_chashSha224->GetHash();
	pthread_mutex_unlock(&_mutex);
	return hash;
}

std::string HashUtils::HashSha256(std::string password)
{
	pthread_mutex_lock(&_mutex);
	this->_chashSha256->HashBuffer(reinterpret_cast<const unsigned char *>(password.c_str()), static_cast<int>(password.length()));
	std::string hash = this->_chashSha256->GetHash();
	pthread_mutex_unlock(&_mutex);
	return hash;
	
}

std::string HashUtils::HashSha1(std::string password)
{
	pthread_mutex_lock(&_mutex);
	this->_chashSha1->HashBuffer(reinterpret_cast<const unsigned char *>(password.c_str()), static_cast<int>(password.length()));
	std::string hash = this->_chashSha1->GetHash();
	pthread_mutex_unlock(&_mutex);
	return hash;

}

std::string HashUtils::HashCrc32(std::string password)
{
	pthread_mutex_lock(&_mutex);
	this->_chashCrc32->HashBuffer(reinterpret_cast<const unsigned char *>(password.c_str()), static_cast<int>(password.length()));
	 
	std::string hash = this->_chashCrc32->GetHash();
	pthread_mutex_unlock(&_mutex);
	return hash;
}

void HashUtils::Kill()
{
	if (HashUtils::_instance != nullptr)
	{
		delete _instance;
	}
}

HashUtils::HashUtils()
{
	this->_chashCrc32 = new CHashCrc32();
	this->_chashSha1 = new CHashSha1();
	_mutex = PTHREAD_MUTEX_INITIALIZER;

	this->_chashShaMD5 = new CHashMd5();
	this->_chashSha224 = new CHashSha224();
	this->_chashSha256 = new CHashSha256();
}


HashUtils::~HashUtils()
{
	pthread_mutex_destroy(&_mutex);

	//TODO fix deletes
	delete this->_chashCrc32;
	delete this->_chashShaMD5;
	delete this->_chashSha256;
	delete this->_chashSha224;
	delete this->_chashSha1;
	
}
