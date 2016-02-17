#pragma once
#include <string>
#include "CSocketIp4.h"

class TcpClient
{
	public:
		TcpClient();
		~TcpClient();
		bool ConnectToHost(const std::string host, int port = 80);
		void ShowEndpointInfos();
		void SendHttpRequest(const std::string host, std::string response = "");
		void WaitForResponse();
		void ReceiveResponse();
		void CloseConnection();
		void DisplayResults() const;
		void ExampleRun();

	private:
		CSocketIp4 _socket;
		std::string _response;
		int _recvCount;
		char _buffer[1024];
		bool _isRunning;
};

