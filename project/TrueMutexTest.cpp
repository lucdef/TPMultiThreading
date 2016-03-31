#include "TrueMutexTest.hpp"
#include <pthread.h>
#include <ostream>
#include <iostream>
#include <string>
#include "TrueMutex.hpp"


struct arg_t
{
	int target;
	TrueMutex mutex;
};

void *AddFunc(void *p_arg) {
	arg_t *target = reinterpret_cast<arg_t*>(p_arg);
	for (int i = 0; i < 50000; i++) {
		target->mutex.Lock();
		std::cout << std::to_string(++target->target) << std::endl;
		target->mutex.Unlock();
	}
	return nullptr;
}

void *SubFunc(void *p_arg) {
	arg_t *target = reinterpret_cast<arg_t*>(p_arg);
	for (int i = 0; i < 50000; i++) {
		target->mutex.Lock();
		std::cout << std::to_string(--target->target) << std::endl;
		target->mutex.Unlock();
	}
	return nullptr;
}

int TrueMutexTest::TestMutex()
{
	const int halfThreadsCpt = 3;
	const int threadsCpt = halfThreadsCpt * 2;
	int i = 0;
	pthread_t t[threadsCpt];
	void *result = nullptr;
	arg_t targetStruct;
	targetStruct.target = 0;
	targetStruct.mutex.Init();


	std::cout << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;

	std::cout << "** Creating thread..." << std::endl;

	for (i = 0;  i < threadsCpt; ++i)
	{
		if (i % 2 == 0)
		{
			if (pthread_create(&t[i], nullptr, SubFunc, reinterpret_cast<arg_t*>(&targetStruct)) != 0) {
				std::cerr << "** FAIL " << std::to_string(i) << std::endl;
				return 0;
			}
		}
		else
		{
			if (pthread_create(&t[i], nullptr, AddFunc, reinterpret_cast<arg_t*>(&targetStruct)) != 0) {
				std::cerr << "** FAIL " << std::to_string(i) << std::endl;
				return 0;
			}
		}
		std::cout << "** Thread " << std::to_string(i) << " creation OK" << std::endl;
	}

	/*if (pthread_create(&t1, nullptr, AddFunc, reinterpret_cast<arg_t*>(&targetStruct)) != 0) {
		std::cerr << "** FAIL 1" << std::endl;
		return 0;
	}
	std::cout << "** Thread 1 creation OK" << std::endl;*/

	std::cout << "** Waiting..." << std::endl;
	for (i = 0; i < threadsCpt; ++i)
		pthread_join(t[i], &result);
	//pthread_join(t2, &result);

	std::string msg = "Test ";
	if (targetStruct.target == 0)
		msg += "SUCCEEDED ";
	else
		msg += "FAILED ";
	
	msg += " (expected 0, got " + std::to_string(targetStruct.target) + ")";

	std::cout << msg << std::endl;
	return 1;
}
