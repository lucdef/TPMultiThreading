#pragma once
#include "ITcpClient.hpp"
#include "CSocketIp4.h"
#include <deque>

class TcpClient : public ITcpClient
{
	public:
		TcpClient();
		~TcpClient();
		bool ConnectToHost(const std::string host, int port = 80);
		void ShowEndpointInfos();
		void SendHttpRequest(const std::string host, std::string response = "");
		void WaitForResponse();
		void ReceiveResponse(const bool display);
		void CloseConnection();
		void DisplayResults(std::string data = "") const;
		void ExampleRun();
		std::string GetResponse();

	private:
		CSocketIp4 _socket;
		int _recvCount;
		char _buffer[1024];
		bool _isRunning;
		std::deque<std::string> _responses;
};

