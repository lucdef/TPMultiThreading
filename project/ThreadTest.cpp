#include "ThreadTest.hpp"
#include <iostream>
#include <pthread.h>
#include "stdafx.h"
#include <string>

ThreadTest::ThreadTest()
{
}


ThreadTest::~ThreadTest()
{
}

void* threadFunction(void *p_arg)
{
	char *text = reinterpret_cast<char*>(p_arg);

	std::cout << "Hello from thread, text is: " << text << std::endl;

	return nullptr;
}

int ThreadTest::Start(const int nbThreads)
{
	pthread_t[5] id;

	for (int i = 0; i < 5; ++i)
	{
		if (pthread_create(&id[i], nullptr, threadFunction, "Ayo") != 0)
			std::cerr << "** Failed to create thread " + std::to_string(id.x) << std::endl;
		else
		{
			void *result = nullptr;
			pthread_join(id, &result);
		}
	}

	return 0;
}