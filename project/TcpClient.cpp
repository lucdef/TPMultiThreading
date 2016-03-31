#include "TcpClient.hpp"
#include <iostream>


TcpClient::TcpClient() :
	_socket(),
	_recvCount(0),
	_isRunning(false),
	_responses()
{
}


TcpClient::~TcpClient()
{
}

bool TcpClient::ConnectToHost(const std::string host, int port)
{
	_socket.InitEngine();

	// Connect to remote host
	std::cout << std::endl;
	std::cout << "Connecting to " << host << std::endl;
	_socket.Connect(host.c_str(), port, 10000);

	// TODO: handle errors

	return true;
}

void TcpClient::ShowEndpointInfos()
{
	// Show information about endpoints
	std::cout << "Getting endpoints information..." << std::endl;
	std::cout << "- local socket is bound to IPv4 " << _socket.GetLocalEndpointIp() << " on port " << _socket.GetLocalEndpointPort() << std::endl;
	std::cout << "- remote socket is bound to IPv4 " << _socket.GetRemoteEndpointIp() << " on port " << _socket.GetRemoteEndpointPort() << std::endl;
}

void TcpClient::SendHttpRequest(const std::string host, std::string data)
{
	// Send HTTP request
	if (data.length() == 0)
	{
		data = "HELLO-HOW-SHOULD-I-WORK";
	}

	std::cout << "Sending request..." << std::endl;
	_socket.Send(data.c_str(), static_cast<unsigned short>(data.length()), NO_TIMEOUT);
}

void TcpClient::WaitForResponse()
{
	// Wait for response
	std::cout << "Waiting for response..." << std::endl;
	_socket.WaitForRead(NO_TIMEOUT);
}

void TcpClient::ReceiveResponse(const bool display)
{
	// Receive whole response
	std::string response = "";
	bool noError = true;

	const int bufferSize = sizeof(_buffer);
	do {
		memset(_buffer, 0, bufferSize);
		std::cout << "Receiving response part..." << std::endl;
		try {
			_recvCount = _socket.Recv(_buffer, bufferSize, NO_TIMEOUT);
			response += _buffer;
			if (_recvCount < bufferSize)
				_recvCount = 0;
		}
		catch (CBrokenSocketException) {
			std::cout << "Connection closed by remote host..." << std::endl;
			_recvCount = 0;
			noError = false;
		}
	} while (_recvCount > 0);

	if (noError)
	{
		_responses.push_back(response);
		DisplayResults(response);
	}
}

void TcpClient::CloseConnection()
{
	// Cleanly close the connection
	std::cout << "Disconnecting..." << std::endl;
	_socket.Shutdown();
}

void TcpClient::DisplayResults(std::string data) const
{
	if (data == "")
		data = _responses.back();

	// Display result
	std::cout << "And the response is..." << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << data << std::endl;
	std::cout << "----------------------------------------" << std::endl;
}

void TcpClient::ExampleRun()
{
	const std::string host = "127.0.0.1";
	int port = 666;

	ConnectToHost(host, port);
	ShowEndpointInfos();
	_isRunning = true;

	while (_isRunning)
	{
		std::string request;
		
		std::cout << "Enter request: ";
		getline(std::cin, request);
		
		if (request == "END")
			_isRunning = false;
		else if (request == "")
			request = "HELLO-HOW-SHOULD-I-WORK";

		SendHttpRequest(host, request);

		WaitForResponse();
		
		ReceiveResponse(true);

		//DisplayResults();
		//_isRunning = false;
	}
	
	CloseConnection();
}

std::string TcpClient::GetResponse()
{
	if (this->_responses.size() == 0)
	{
		throw _CException("No Hash type specified in the command line !!! Exit !!!", 0);
	}
	std::string response = this->_responses.at(0);
	this->_responses.pop_front();
	return response;
}
