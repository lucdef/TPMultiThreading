#pragma once
#include <string>
#include "CSocketIp4.h"

class ITcpClient
{
public:
	ITcpClient() {};
	virtual ~ITcpClient() {};
	virtual bool ConnectToHost(const std::string host, int port) = 0;
	//void ShowEndpointInfos();
	virtual void SendHttpRequest(const std::string host, std::string data) = 0;
	virtual void WaitForResponse() = 0;
	virtual void ReceiveResponse() = 0;
	void CloseConnection();
	virtual void DisplayResults() const = 0;
	virtual void ExampleRun() {};
};

