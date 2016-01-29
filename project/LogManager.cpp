#include "LogManager.h"
#include <sstream>
#include "CException.h"



LogManager::LogManager()
{
	_logFile = new CFileText("C:\\Users\\lucdef\\Desktop\\TPMultiThreadingLog.txt", EFileOpenMode::append);
	_logFile->Open(EFileOpenMode::append);
}


LogManager::~LogManager()
{
	_logFile->Flush();
	_logFile->Close();
}

bool LogManager::LogWarning(int idThread, std::string message)
{
	std::string criticite = "[WARNING]";
	bool result = log(idThread, message, criticite);
	return result;
}

bool LogManager::LogError(int idThread, std::string message)
{
	std::string criticite = "[ERROR]";
	bool result = log(idThread, message, criticite);
	return result;
}

bool LogManager::LogInfo(int idThread, std::string message)
{
	std::string criticite = "[INFO]";
	bool result = log(idThread, message, criticite);
	return result;
}

bool LogManager::log(int idThread, std::string message, std::string criticite)
{
	CDateTime date;
	std::string dateString;
	std::ostringstream stream;
	date.Now();
	
	dateString = LogManager::DateToString(date);
	stream << criticite << " " << dateString << " Thread:" << idThread << " " << message << std::endl;
	try{
	_logFile->AppendLine(stream.str(), EFileEOL::Windows);
	return true;
}
catch (CException &e) {			// & is IMPORTANT
	std::cerr << "** --- EXCEPTION THROWN ---" << std::endl;
	std::cerr << "** Type: " << e.GetType() << std::endl;
	std::cerr << "** Message: " << e.GetErrorMessage() << std::endl;
	std::cerr << "** Error code: " << e.GetErrorCode() << std::endl;
	std::cerr << "** Fault location: " << e.GetFaultLocation() << std::endl;
	return false;
}

	
}
std::string LogManager::DateToString(CDateTime d)
{
	std::string str;
	std::ostringstream stream;
	stream << d.m_wYear << "-" << d.m_wMonth << "-" << d.m_wDay << " @ " << d.m_wHour << ":" << d.m_wMinute << ":" << d.m_wSecond << "." << d.m_wMilliseconds << std::endl;

	return stream.str();
}
