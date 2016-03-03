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
		bool SendData(std::string data = "");
		void Run(unsigned short port);
		void DisconnectClient(CSocketIp4 *remoteClient);
		void StopServer();
		std::string GetPatternFromData(const std::string &tmp, const std::string &pattern);
		std::string GetFounderFromData(const std::string &tmp);


		static std::string TestGetPassFromData();
		static std::string TestGetFounderFromData();

	private:
		static const short MAX_CONNECTION = 10;
		static const std::string PASS_PATTERN;
		static const std::string FOUNDER_PATTERN;

		CSocketIp4 _socket;
		CSocketIp4 *_remoteClient;
		char _buffer[1024];
		int _recvCount;
		std::string _request;
		std::string _response;
		bool _isRunning;
};

