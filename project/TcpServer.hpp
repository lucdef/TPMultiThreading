#pragma once

#include "CSocketIp4.h"


class OGlobal;

class TcpServer
{
	public:
		TcpServer();
		~TcpServer();
		int StartServer();
		void ParseHttp(const std::string data);
		void ReceiveData();
		void SendData(std::string data = "");
		void Run(unsigned short port);
		void DisconnetClient(CSocketIp4 *remoteClient);
		void StopServer();

	private:
		CSocketIp4 _socket;
		CSocketIp4 *_remoteClient;
		char _buffer[1024];
		int _recvCount;
		std::string _request;
		std::string _response;
};

