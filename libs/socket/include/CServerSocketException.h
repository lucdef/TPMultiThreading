#pragma once

#include <iostream>
#include <string>

#include "CException.h"


#define _CServerSocketException(message, errorCode)			CServerSocketException(message, errorCode, __FILE__, __LINE__, __FUNCTION__)


class CServerSocketException : public CException {
public:
	static const std::string CLASS_TYPE;

public:
	CServerSocketException( const std::string &p_message,
							const unsigned int p_errorCode,
							const std::string &p_file,
							const unsigned int p_line,
							const std::string &p_function );
};
