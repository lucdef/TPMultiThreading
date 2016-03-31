#pragma once
#include <string>
#include <iostream>
#include <ctime>
#include "EFileOpenMode.h"
#include "CDateTime.h"
#include "CFileText.h"
#include "TrueMutex.hpp"
#include "singleton.h"
class LogManager
{
public:
	static LogManager* GetInstance();
	static void Kill();
	bool LogWarning(int idThread, const std::string& message);
	bool LogError(int idThread, const std::string& message);
	bool LogInfo(int idThread, const std::string& message);


private:
	~LogManager();
	LogManager();
	static LogManager *_instance;
	bool log(int idThread, const std::string &message, const std::string &criticite);
	static std::string DateToString(const CDateTime &date);
	CFileText *_logFile;
	//TrueMutex *_mutex;
	pthread_mutex_t _mutex;
};

