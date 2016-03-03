#pragma once

#include "CSocketIp4.h"

class WinCSocket : public CSocketIp4
{
public:
	WinCSocket();
	~WinCSocket();

	//Select();
};

