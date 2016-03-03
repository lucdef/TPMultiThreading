// TcpClientTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TcpClient.hpp"

int main()
{
	const std::string host = "localhost";
	const short port = 666;
	TcpClient *client = new TcpClient();

	//client.ExampleRun();

    return 0;
}

