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
	bool LogWarning(int idThread, std::string message);
	bool LogError(int idThread, std::string message);
	bool LogInfo(int idThread, std::string message);


private:
	~LogManager();
	LogManager();
	static LogManager *_instance;
	bool log(int idThread, std::string message, std::string criticite);
	static std::string DateToString(CDateTime date);
	CFileText *_logFile;
	TrueMutex *_mutex;
};

