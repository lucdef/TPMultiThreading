#pragma once

#include "CSocketIp4.h"

class TcpServer
{
	public:
		TcpServer();
		~TcpServer();
		int StartServer();
		void ParseHttp(const std::string data) const;
		void ReceiveResponse();
		void SendData(std::string data = "");
		void Run(unsigned short port);
		void DisconnetClient(CSocketIp4 *remoteClient);
		void StopServer();

	private:
		CSocketIp4 _socket;
		CSocketIp4 *_remoteClient;
		char buffer[1024];
		int recvCount;
		std::string request;
};

