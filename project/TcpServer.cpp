#include "TcpServer.hpp"
#include "OGlobal.hpp"
#include "utils.h"


TcpServer::TcpServer() :
	_socket(),
	_remoteClient(nullptr),
	_request(),
	_response(),
	_isRunning(false)
{
}

TcpServer::~TcpServer()
{
}

int TcpServer::StartServer()
{
	//_socket.InitEngine();
	//_socket.CreateServer(666, MAX_CONNECTION);

	// TODO
	return 0;
}

std::string TcpServer::ParseHttp(const std::string data)
{
	OGlobal *_ordonnanceur = OGlobal::GetInstance();
	// For pleasure, let's do a quick HTTP parsing
	std::cout << "[TcpServer] - parsing request..." << std::endl;
	std::string tmp = data, response = "";
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


std::string TcpServer::ReceiveData()
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

	if (data.length() == 0)
	{
		data = _response;
	}

	_remoteClient->Send(data.c_str(), static_cast<unsigned short>(data.length()), NO_TIMEOUT);
}

void TcpServer::Run(unsigned short port)
{
	std::cout << std::endl;
	std::cout << "[TcpServer] Creating HTTP server on port 666..." << std::endl;

	_socket.InitEngine();
	_socket.CreateServer(port, MAX_CONNECTION);

	_isRunning = true;
	std::cout << "[TcpServer] Waiting for connections..." << std::endl;
	_remoteClient = dynamic_cast<CSocketIp4 *>(_socket.Accept());
	std::cout << "[TcpServer] Accepted incoming connection from " << _remoteClient->GetRemoteEndpointIp() << " on port " << _remoteClient->GetRemoteEndpointPort() << std::endl;
	
	while (_isRunning) { // TODO socket IO exceptions /!\
		int recvCount = 0;
		char buffer[1024];
				
		// Receive whole response with 100ms timeout
		// !! WARNING !! a nicer way to handle this request is to check for end-of-request instead of foolishly wait for 100ms
		std::string request = ReceiveData();

		// For pleasure, let's do a quick HTTP parsing
		std::string response = ParseHttp(request);


		// Send him the same information everytime
		// Oww! crap! No doctype ... and crappy headers too. But it is working, so enjoy.
		SendData(response);

		//DisconnectClient(_remoteClient);
	}
	DisconnectClient(_remoteClient);
	StopServer();

	return;
}

void TcpServer::DisconnectClient(CSocketIp4 *rremoteClient)
{
	// Disconnect
	//if (rremoteClient == nullptr)
	//	return;

	//rremoteClient->Shutdown();
	//delete(rremoteClient);
	//rremoteClient = nullptr;


	if (_remoteClient == nullptr)
		return;

	_remoteClient->Shutdown();
	delete(_remoteClient);
	_remoteClient = nullptr;
}

void TcpServer::StopServer()
{
	_isRunning = false;

	if (_isRunning)
	{
		// here if called by TcpServer::Run(unsigned short)
		_socket.Shutdown();
		std::cout << "Server stopped" << std::endl;
	}
}