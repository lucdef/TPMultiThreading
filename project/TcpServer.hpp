#pragma once

#include "CSocketIp4.h"


class OGlobal;

class TcpServer
{
	public:
		TcpServer();
		~TcpServer();
		int StartServer();
		std::string ParseHttp(const std::string data);
		std::string ReceiveData();
		void SendData(std::string data = "");
		void Run(unsigned short port);
		void DisconnectClient(CSocketIp4 *remoteClient);
		void StopServer();

	private:
		static const short MAX_CONNECTION = 10;

		CSocketIp4 _socket;
		CSocketIp4 *_remoteClient;
		char _buffer[1024];
		int _recvCount;
		std::string _request;
		std::string _response;
		bool _isRunning;
};

