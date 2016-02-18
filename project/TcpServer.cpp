#include "TcpServer.hpp"
#include "OGlobal.hpp"
#include "utils.h"



TcpServer::TcpServer() :
	_serverSocket()
{
	// TODO: init
	//for (int i = 0; i < MAX_CONNECTION; ++i)
	//	_remoteClient[i] = ???;
}

TcpServer::~TcpServer()
{
}

int TcpServer::StartServer()
{
	//_socket.InitEngine();
	//_socket.CreateServer(666, MAX_CONNECTION);

	_remoteClient = nullptr;

	// TODO
	return 0;
}

const std::string TcpServer::ParseHttp(const std::string &data)
{
	OGlobal *_ordonnanceur = OGlobal::GetInstance();
	std::string response = "";

	// For pleasure, let's do a quick HTTP parsing
	std::cout << "[TcpServer] - parsing request..." << std::endl;
	std::string tmp = data;
	if (tmp.find('\r') > 0) {
		tmp = tmp.substr(0, tmp.find('\r'));
	}
	std::cout << "[TcpServer] - request is \"" << tmp << "\"" << std::endl;

	if (tmp == "HELLO-HOW-SHOULD-I-WORK")
	{
		// TODO: stack the response in fifo resp
		response = "WORKING-CONTEXT HASH=" + _ordonnanceur->GetHash() + " ALGO=" + _ordonnanceur->GetAlgo() + " ALPHABET=" + _ordonnanceur->GetAlphabet();
	}
	else if ( Utils::StringContains(tmp, "NEW-CHUNK-PLEASE" ))
	{
		//_ordonnanceur->replyChunk();
		response = "NEW-CHUNK-FOR-YOU=" + _ordonnanceur->GetNextChunkBegin();
	}
	else
	{
		response = "UNKNOWN COMMAND";
	}

	return response;
}


const std::string TcpServer::ReceiveData()
{
	// Receive whole response with 100ms timeout
	// !! WARNING !! a nicer way to handle this request is to check for end-of-request instead of foolishly wait for 100ms
	std::cout << "[TcpServer] - receiving its request..." << std::endl;
	std::string request = "";
	
	do {
		memset(_buffer, 0, sizeof(_buffer));
		try {
			_recvCount = _remoteClient->Recv(_buffer, sizeof(_buffer), NO_TIMEOUT);
			request += _buffer;
		}
		catch (CBrokenSocketException) {
			std::cout << "[TcpServer] Connection closed by remote host..." << std::endl;
			_recvCount = 0;
		}
	} while (_recvCount > 0 && _remoteClient->WaitForRead(100) != SOCKET_TIMEOUT);

	return request;
}

void TcpServer::SendData(std::string data)
{
	// Send him the same information everytime
	// Oww! crap! No doctype ... and crappy headers too. But it is working, so enjoy.
	std::cout << "[TcpServer] - sending fake page..." << std::endl;

	//if (data.length() == 0)
	//{
	//	data = _response;
	//}

	_remoteClient->Send(data.c_str(), static_cast<unsigned short>(data.length()), NO_TIMEOUT);
}

void TcpServer::Run(unsigned short port)
{
	static int clientId = 0;

	std::cout << std::endl;
	std::cout << "[TcpServer] Creating HTTP server on port 666..." << std::endl;

	_serverSocket.InitEngine();
	_serverSocket.CreateServer(port, 5);
	
	for (;;) {
		int recvCount = 0;
		char buffer[1024];

		//CSocketIp4 *remoteClient = &(remoteClient[clientId]);
		std::cout << "[TcpServer] Waiting for connections..." << std::endl;
		_remoteClient = dynamic_cast<CSocketIp4 *>(_serverSocket.Accept());

		std::cout << "[TcpServer] Accepted incoming connection from " << _remoteClient->GetRemoteEndpointIp() << " on port " << _remoteClient->GetRemoteEndpointPort() << std::endl;
		++clientId;
	//for (;;) {
		// Receive whole response with 100ms timeout
		// !! WARNING !! a nicer way to handle this request is to check for end-of-request instead of foolishly wait for 100ms
		std::string request = ReceiveData();

		//if(request == "END")
			//DisconnectClient(_remoteClient);

		// For pleasure, let's do a quick HTTP parsing
		std::string response = ParseHttp(request);


		// Send him the same information everytime
		// Oww! crap! No doctype ... and crappy headers too. But it is working, so enjoy.
		SendData(response);

		DisconnectClient(_remoteClient);
	}
	StopServer();

	return;
}

void TcpServer::DisconnectClient(CSocketIp4 *rremoteClient)
{
	// Disconnect
	rremoteClient->Shutdown();
	delete rremoteClient;
}

void TcpServer::StopServer()
{
	_serverSocket.Shutdown();
}