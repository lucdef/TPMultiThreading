#include "TcpClient.hpp"
#include <iostream>


TcpClient::TcpClient() :
	_socket(),
	_data()
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
	_socket.Connect(host.c_str(), port, 2500);

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

void TcpClient::SendHttpRequest(const std::string host)
{
	// Send HTTP request
	if (_data.length() == 0)
	{
		_data = "GET / HTTP/1.1\r\n"
			"Host: " + host + "\r\n"
			"Connection: close\r\n"
			"\r\n";
	}

	std::cout << "Sending request..." << std::endl;
	_socket.Send(_data.c_str(), static_cast<unsigned short>(_data.length()), NO_TIMEOUT);
}

void TcpClient::WaitForResponse()
{
	// Wait for response
	std::cout << "Waiting for response..." << std::endl;
	_socket.WaitForRead(NO_TIMEOUT);
}

void TcpClient::ReceiveResponse()
{
	// Receive whole response
	_data = "";
	do {
		memset(_buffer, 0, sizeof(_buffer));
		std::cout << "Receiving response part..." << std::endl;
		// When Google has finished sending me its data, it closes the connection ; thus I'm getting an exception but it's okay
		try {
			_recvCount = _socket.Recv(_buffer, sizeof(_buffer), NO_TIMEOUT);
			_data += _buffer;
		}
		catch (CBrokenSocketException) {
			std::cout << "Connection closed by remote host..." << std::endl;
			_recvCount = 0;
		}
	} while (_recvCount > 0);
}

void TcpClient::CloseConnection()
{
	// Cleanly close the connection
	std::cout << "Disconnecting..." << std::endl;
	_socket.Shutdown();
}

void TcpClient::DisplayResults() const
{
	// Display result
	std::cout << "And the response is..." << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << _data << std::endl;
	std::cout << "----------------------------------------" << std::endl;
}

void TcpClient::ExampleRun()
{
	const std::string host = "127.0.0.1";
	int port = 666;

	ConnectToHost(host, port);
	ShowEndpointInfos();
	SendHttpRequest(host);
	WaitForResponse();
	ReceiveResponse();
	CloseConnection();
	DisplayResults();
}
