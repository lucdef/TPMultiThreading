#include "TcpServer.hpp"
#include "OGlobal.hpp"
#include "utils.h"
#include "LogManager.h"

#include <regex>

const std::string TcpServer::PASS_PATTERN = "PASS=([^ ]+) ";
const std::string TcpServer::FOUNDER_PATTERN = "FOUND-BY=([^ ]+) ";

TcpServer::TcpServer() :
	_logger(LogManager::GetInstance()),
	_socket(),
	_remoteClient(nullptr),
	_recvCount(0),
	_request(),
	_response(),
	_isRunning(false)
{
}

std::string TcpServer::ParseHttp(const std::string &data) const
{
	if (data == "")
		return data;

	static OGlobal *ordonnanceur = OGlobal::GetInstance();
	// For pleasure, let's do a quick HTTP parsing
	std::cout << "[TcpServer] - parsing request..." << std::endl;
	std::string tmp = data, response;
	if (tmp.find('\r') > 0) {
		tmp = tmp.substr(0, tmp.find('\r'));
	}
	std::cout << "[TcpServer] - request is \"" << tmp << "\"" << std::endl;

	if (tmp == "HELLO-HOW-SHOULD-I-WORK")
	{
		// TODO: stack the response in fifo resp
		response = "WORKING-CONTEXT HASH=" + ordonnanceur->GetHash() + " ALGO=" + ordonnanceur->GetAlgo() + " ALPHABET=" + ordonnanceur->GetAlphabet();
	}
	else if ( Utils::StringContains(tmp, "NEW-CHUNK-PLEASE" ))
	{
		// TODO: get lastHandled from tmp
		std::string startPass = ordonnanceur->GetNextChunkBegin();


		response = "NEW-CHUNK-FOR-YOU=" + startPass;
		
		ordonnanceur->AddGivenChunk(startPass, _remoteClient);
	}
	else if (Utils::StringContains(tmp, "FOUND-PLEASE-EXIT"))
	{
		// get pass from tmp
		std::string pass = Utils::GetPatternFromData(tmp, PASS_PATTERN);
		std::string founder = Utils::GetPatternFromData(tmp, FOUNDER_PATTERN);

		//_ordonnanceur->replyChunk();
		response = "GOOD-JOB";
	}
	else
	{
		response = "UNKNOWN COMMAND";
	}

	return response;
}


std::string TcpServer::ReceiveData()
{
	//_logger = LogManager::GetInstance();

	// Receive whole response with 100ms timeout
	// !! WARNING !! a nicer way to handle this request is to check for end-of-request instead of foolishly wait for 100ms
	std::cout << "[TcpServer] - receiving its request..." << std::endl;
	std::string request = "", logMsg = "";
	
	do {
		memset(_buffer, 0, sizeof(_buffer));
		try {
			_recvCount = _remoteClient->Recv(_buffer, sizeof(_buffer), NO_TIMEOUT);
			request += _buffer;
		}
		catch (CBrokenSocketException) {
			logMsg = "[TcpServer] Connection closed by remote host...";
			std::cout << logMsg << std::endl;
			_recvCount = 0;
			request = "";
		}
	} while (_recvCount > 0 && _remoteClient->WaitForRead(100) != SOCKET_TIMEOUT);


	if(logMsg.length() == 0)
	{
		_logger->LogInfo(2, "Server received '<data>' from <from>");
	}
	else
	{
		_logger->LogError(2, logMsg);
	}

	
	return request;
}

bool TcpServer::SendData(const std::string& data) const
{
	if (data == "")
		return false;

	// Send him the same information everytime
	// Oww! crap! No doctype ... and crappy headers too. But it is working, so enjoy.
	std::cout << "[TcpServer] - sending fake page..." << std::endl;

	if (data.length() == 0)
	{
		return false;
	}

	try
	{
		_remoteClient->Send(data.c_str(), static_cast<unsigned short>(data.length()), NO_TIMEOUT);
		_logger->LogInfo(1, "Server sent '" + data + "' to '" + Utils::GetClientStr(_remoteClient) + "'");
	}
	catch (CException e)
	{
		std::cerr << "ERREUR: " << e.GetErrorMessage() << std::endl;
		_logger->LogError(1, "Server error while sending '" + data + "' to '" + Utils::GetClientStr(_remoteClient) + "'");;
		return false;
	}

	return true;
}



void TcpServer::Run(unsigned short port)
{
	std::cout << std::endl;
	std::cout << "[TcpServer] Creating HTTP server on port" << port << "..." << std::endl;

	_socket.InitEngine();
	_socket.CreateServer(port, MAX_CONNECTION);

	_isRunning = true;
		
	while (_isRunning) { // TODO socket IO exceptions /!\
		int recvCount = 0;
		char buffer[1024];
		
		// Wait for connection
		std::cout << "[TcpServer] Waiting for connections..." << std::endl;
		_remoteClient = dynamic_cast<CSocketIp4 *>(_socket.Accept());
		std::cout << "[TcpServer] Accepted incoming connection from " << _remoteClient->GetRemoteEndpointIp() << " on port " << _remoteClient->GetRemoteEndpointPort() << std::endl;

		_logger->LogInfo(1, "[TcpServer] Accepted incoming connection from " + _remoteClient->GetRemoteEndpointIp() + " on port " + std::to_string(_remoteClient->GetRemoteEndpointPort()));

		try
		{
			// Receive whole response with 100ms timeout
			// !! WARNING !! a nicer way to handle this request is to check for end-of-request instead of foolishly wait for 100ms
			std::string request = ReceiveData();

			// For pleasure, let's do a quick HTTP parsing
			std::string response = ParseHttp(request);


			// Send him the same information everytime
			_isRunning = SendData(response); // TODO send to list of client

			if (response == "GOOD JOB")
				break;
		}
		catch (CSocketIOException e)
		{
			std::cerr << "ERREUR: " << e.GetErrorMessage() << std::endl;
			_logger->LogError(1, "Communication error while handling client " + Utils::GetClientStr(_remoteClient) + " - " + e.GetErrorMessage());
			_isRunning = false;
		}

		DisconnectClient();
	}
	DisconnectClient();
	StopServer();
}

void TcpServer::DisconnectClient()
{
	// Disconnect
	if (_remoteClient == nullptr)
		return;

	_remoteClient->Shutdown();
	delete(_remoteClient);
	_remoteClient = nullptr;

	_logger->LogInfo(1, "Server disconnected '<client>'");
}

void TcpServer::StopServer()
{
	_isRunning = false;

	if (_isRunning)
	{
		// here if called by TcpServer::Run(unsigned short)
		_socket.Shutdown();
		std::cout << "Server stopped" << std::endl;

		_logger->LogInfo(1, "Server asked to stop");
	}
}


std::string TcpServer::TestGetPassFromData()
{
	std::string value = "";
	TcpServer server = TcpServer();


	// Simple regular expression matching
	std::string fnames[] = { "PASS=bon ", "PASS=bon vsrverv", "PASS=bonvfsdv rfedgd ", "PASS=bon'!:;vxf47 " };

	for (const auto &fname : fnames) {
		std::cout << "GOT: '" << Utils::GetPatternFromData(fname, PASS_PATTERN) << "'" << std::endl;
	}

	return value;
}

std::string TcpServer::TestGetFounderFromData()
{
	std::string value = "";
	TcpServer server = TcpServer();


	// Simple regular expression matching
	std::string fnames[] = { "FOUND-BY=bon ", "FOUND-BY=bon vsrverv", "FOUND-BY=bonvfsdv rfedgd ", "FOUND-BY=bon'!:;vxf47 " };

	for (const auto &fname : fnames) {
		std::cout << "GOT: '" << Utils::GetPatternFromData(fname, FOUNDER_PATTERN) << "'" << std::endl;
	}

	return value;
}