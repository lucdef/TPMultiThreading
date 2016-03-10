#pragma once
#include <string>

class ITcpClient
{
public:
	ITcpClient() {};
	virtual ~ITcpClient() {};
	virtual bool ConnectToHost(const std::string host, int port) = 0;
	//void ShowEndpointInfos();
	virtual void SendHttpRequest(const std::string host, std::string data) = 0;
	virtual void WaitForResponse() = 0;
	virtual void ReceiveResponse(const bool display) = 0;
	void CloseConnection();
	virtual void DisplayResults(std::string data) const = 0;
	virtual void ExampleRun() {};
};

