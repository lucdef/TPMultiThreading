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
	bool LogWarning(int idThread, const std::string& message) const;
	bool LogError(int idThread, const std::string& message) const;
	bool LogInfo(int idThread, const std::string& message) const;


private:
	~LogManager();
	LogManager();
	static LogManager *_instance;
	bool log(int idThread, const std::string &message, const std::string &criticite) const;
	static std::string DateToString(const CDateTime &date);
	CFileText *_logFile;
	TrueMutex *_mutex;
};

