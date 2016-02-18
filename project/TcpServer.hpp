#pragma once

#include "CSocketIp4.h"


class OGlobal;

class TcpServer
{
	public:
		TcpServer();
		~TcpServer();
		int StartServer();
		const std::string ParseHttp(const std::string &data);
		const std::string ReceiveData();
		void SendData(std::string data = "");
		void Run(unsigned short port);
		void DisconnectClient(CSocketIp4 *remoteClient);
		void StopServer();

	private:
		static const short MAX_CONNECTION = 5;
		CSocketIp4 _serverSocket;
		CSocketIp4* _remoteClient; // TODO client count
		char _buffer[1024];
		int _recvCount;
		//std::string _request;
		//std::string _response;
};

