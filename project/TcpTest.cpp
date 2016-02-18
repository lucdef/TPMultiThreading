#include "TcpTest.hpp"
#include "TcpServer.hpp"
#include "TcpClient.hpp"
#include "pthread.h"
#include "utils.h"
#include "OGlobal.hpp"

TcpTest::TcpTest()
{
}


TcpTest::~TcpTest()
{
}

void *ThreadServerFunc(void *p_arg)
{
	try
	{
		OGlobal *ordonnanceur = OGlobal::GetInstance(5, 3, "sha256", "XXXXXXXXXX", "0123456789");
		ordonnanceur->StartServer(666);
	}
	catch (CException &e) {
		std::cerr << "** --- EXCEPTION THROWN FROM SERVER ---" << std::endl;
		std::cerr << "** Type: " << e.GetType() << std::endl;
		std::cerr << "** Message: " << e.GetErrorMessage() << std::endl;
		std::cerr << "** Error code: " << e.GetErrorCode() << std::endl;
		std::cerr << "** Fault location: " << e.GetFaultLocation() << std::endl;
	}

	std::cout << "Appuyer sur <Enter> pour continuer";
	std::cin.get();
	return 0;
}

void *ThreadClientFunc(void *p_arg)
{
	try
	{
		TcpClient client = TcpClient();
		//int ret = serv.StartServer();
		client.ExampleRun();
	}
	catch (CException &e) {
		std::cerr << "** --- EXCEPTION THROWN FROM CLIENT ---" << std::endl;
		std::cerr << "** Type: " << e.GetType() << std::endl;
		std::cerr << "** Message: " << e.GetErrorMessage() << std::endl;
		std::cerr << "** Error code: " << e.GetErrorCode() << std::endl;
		std::cerr << "** Fault location: " << e.GetFaultLocation() << std::endl;
	}

	std::cout << "ThreadClient finished !";
	return 0;
}

int TcpTest::TestMain()
{
	pthread_t ServerThread, ClientThread;
	void *result = nullptr;

	std::cout << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;

	std::cout << "** Creating thread..." << std::endl;
	if (pthread_create(&ServerThread, nullptr, ThreadServerFunc, reinterpret_cast<void *>(1)) != 0) {
		std::cerr << "** FAIL Server1" << std::endl;
		return 1;
	}
	else {
		std::cout << "** ThreadServer creation OK" << std::endl;
	}

	Utils::mySleep(5000);

	if (pthread_create(&ClientThread, nullptr, ThreadClientFunc, reinterpret_cast<void *>(2)) != 0) {
		std::cerr << "** FAIL Client" << std::endl;
		return 1;
	}
	else {
		std::cout << "** ThreadClient creation OK" << std::endl;
	}

	std::cout << "** Waiting..." << std::endl;
	pthread_join(ServerThread, &result);
	pthread_join(ClientThread, &result);

	return 0;
}
