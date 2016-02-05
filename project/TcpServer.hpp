#pragma once

#include "CSocketIp4.h"

class TcpServer
{
	public:
		TcpServer();
		~TcpServer();
		int StartServer();
		void Run(unsigned short port);
		void DisconnetClient(CSocketIp4 *remoteClient);
		void StopServer();

	private:
		CSocketIp4 _socket;
		CSocketIp4 *_remoteClient;
};

