#include "TcpServer.hpp"

#define MAX_CONNECTION 10

TcpServer::TcpServer() :
	_socket()
{
	_remoteClient = nullptr;
}

TcpServer::~TcpServer()
{
}

int TcpServer::StartServer()
{
	//_socket.InitEngine();
	//_socket.CreateServer(666, MAX_CONNECTION);

	return 0;
}


void TcpServer::Run(unsigned short port)
{
	std::cout << std::endl;
	std::cout << "Creating HTTP server on port 666..." << std::endl;

	_socket.InitEngine();
	_socket.CreateServer(port, 5);
	for (;;) {
		std::string data, request, tmp;
		int recvCount = 0;
		char buffer[1024];


		std::cout << "Waiting for connections..." << std::endl;
		_remoteClient = dynamic_cast<CSocketIp4 *>(_socket.Accept());

		std::cout << "Accepted incoming connection from " << _remoteClient->GetRemoteEndpointIp() << " on port " << _remoteClient->GetRemoteEndpointPort() << std::endl;

		// Receive whole response with 100ms timeout
		// !! WARNING !! a nicer way to handle this request is to check for end-of-request instead of foolishly wait for 100ms
		std::cout << "- receiving its request..." << std::endl;
		request = "";
		do {
			memset(buffer, 0, sizeof(buffer));
			try {
				recvCount = _remoteClient->Recv(buffer, sizeof(buffer), NO_TIMEOUT);
				request += buffer;
			}
			catch (CBrokenSocketException) {
				std::cout << "Connection closed by remote host..." << std::endl;
				recvCount = 0;
			}
		} while (recvCount > 0 && _remoteClient->WaitForRead(100) != SOCKET_TIMEOUT);

		// For pleasure, let's do a quick HTTP parsing
		std::cout << "- parsing request..." << std::endl;
		tmp = request;
		if (tmp.find('\r') > 0) {
			tmp = tmp.substr(0, tmp.find('\r'));
		}
		std::cout << "- request is \"" << tmp << "\"" << std::endl;

		// Send him the same information everytime
		// Oww! crap! No doctype ... and crappy headers too. But it is working, so enjoy.
		std::cout << "- sending fake page..." << std::endl;
		data = "HTTP/1.1 200 OK\r\n"
			"Host: hello.it.s.me\r\n"
			"Server: my-server\r\n"
			"Content-type: text/html\r\n"
			"Connection: close\r\n"
			"\r\n"
			"<html>"
			"  <head>"
			"    <title>Hello from local HTTP server</title>"
			"    <style type='text/css'>"
			"    h1 { color: navy; font-variant: small-caps; }"
			"    pre { background-color: #EEE; border: 1px solid #CCC; }"
			"    </style>"
			"  </head>"
			"  <body>"
			"    <h1>You should know that ...</h1>"
			"    <ul>"
			"      <li>you just created your first HTTP server</li>"
			"      <li>your client connected from IP address <strong>" + _remoteClient->GetRemoteEndpointIp() + "</strong> and port <strong>" + std::to_string(_remoteClient->GetRemoteEndpointPort()) + "</strong></li>"
			"    </ul>"
			"    <h1>Your client sent those headers</h1>"
			"    <pre>" + request + "</pre>"
			"  </body>"
			"</html>";
		_remoteClient->Send(data.c_str(), static_cast<unsigned short>(data.length()), NO_TIMEOUT);

		DisconnetClient(_remoteClient);
	}
	StopServer();

	return;
}

void TcpServer::DisconnetClient(CSocketIp4 *rremoteClient)
{
	// Disconnect
	rremoteClient->Shutdown();
	delete rremoteClient;
}

void TcpServer::StopServer()
{
	_socket.Shutdown();
}