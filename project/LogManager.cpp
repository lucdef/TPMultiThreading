#include "LogManager.h"
#include <sstream>
#include "CException.h"
#include "TrueMutex.hpp"


LogManager* LogManager::_instance = nullptr;


LogManager::LogManager()
{
	
	_logFile = new CFileText("..\\TPMultiThreading.log", EFileOpenMode::append);
	
	_logFile->Open(EFileOpenMode::append);
	_mutex = new TrueMutex();
	_mutex->Init();
}


LogManager::~LogManager()
{
	_logFile->Flush();
	_logFile->Close();
	delete _logFile;
	delete _mutex;
}



LogManager* LogManager::GetInstance()
{
	if (_instance == nullptr)
	{
		LogManager::_instance = new LogManager();
	}
	return LogManager::_instance;
}

void LogManager::Kill()
{
	if(_instance != nullptr)
		delete(_instance);
}

bool LogManager::LogWarning(int idThread, const std::string &message) const
{
	std::string criticite = "[WARNING]";
	bool result = log(idThread, message, criticite);
	return result;
}

bool LogManager::LogError(int idThread, const std::string &message) const
{
	std::string criticite = "[ERROR]";
	bool result = log(idThread, message, criticite);
	return result;
}

bool LogManager::LogInfo(int idThread, const std::string &message) const
{
	std::string criticite = "[INFO]";
	bool result = log(idThread, message, criticite);
	return result;
}

bool LogManager::log(int idThread, const std::string &message, const std::string &criticite) const
{
	_mutex->Lock();
	CDateTime date;
	std::string dateString;
	std::ostringstream stream;
	date.Now();
	
	dateString = LogManager::DateToString(date);
	stream << criticite << " " << dateString << " Thread:" << idThread << " " << message << std::endl;
	try{
		_logFile->AppendLine(stream.str(), EFileEOL::Windows);
		_logFile->Flush();
		_mutex->Unlock();
		return true;
	}
	catch (CException &e) {			// & is IMPORTANT
		_mutex->Unlock();
		std::cerr << "** --- EXCEPTION THROWN ---" << std::endl;
		std::cerr << "** Type: " << e.GetType() << std::endl;
		std::cerr << "** Message: " << e.GetErrorMessage() << std::endl;
		std::cerr << "** Error code: " << e.GetErrorCode() << std::endl;
		std::cerr << "** Fault location: " << e.GetFaultLocation() << std::endl;
		return false;
	}
}

std::string LogManager::DateToString(const CDateTime &d)
{
	std::ostringstream stream;
	stream << d.m_wYear << "-" << d.m_wMonth << "-" << d.m_wDay << " @ " << d.m_wHour << ":" << d.m_wMinute << ":" << d.m_wSecond << "." << d.m_wMilliseconds << std::endl;

	return stream.str();
}
