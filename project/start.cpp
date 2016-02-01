#include "stdafx.h"
#include <iostream>
#include <deque>

#include "utils.h"
#include "CException.h"
#include "PasswordChunk.h"

#include <pthread.h>
#include <Windows.h>

#include "TrueMutex.hpp"

void ExtractCommandLine( int argc, const char *argv[] )	{
	// Command line settings
	// Tip: good to be stored in a singleton dedicated to configuration	--> see singleton.h
	std::string hash, algo, alphabet, masterIpAddress;
	unsigned int chunkSize = 0;
	bool runningAsSlave = false;

	// Extract command line
	try {
		HashCrackerUtils::ParseCommandLine(argc, argv, hash, algo, alphabet, chunkSize, masterIpAddress, runningAsSlave);
	}
	catch(CException &ex) {
		std::cerr << "** Command line extraction failed at \"" << ex.GetFaultLocation() << "\" with error code " << ex.GetErrorCode() << " and message \"" << ex.GetErrorMessage() << "\"" << std::endl;
		return;
	}

	// Show information
	std::cout << "--- INFORMATION GOT FROM COMMAND LINBE ---" << std::endl;
	std::cout << "Mode: " << (runningAsSlave ? "slave" : "master") << std::endl;
	if( runningAsSlave ) {
		std::cout << "-ip " << masterIpAddress << std::endl;
	}
	else {
		std::cout << "-hash " << hash << std::endl;
		std::cout << "-algo " << algo << std::endl;
		std::cout << "-alphabet " << alphabet << std::endl;
		std::cout << "-chunksize " << chunkSize << std::endl;
	}
}


void GeneratePasswords() {
	char password[64] = "";
	std::string testAlphabet = "0123456789";

	strcpy_s(password, sizeof(password), "");
	for( int i = 0; i < 2500; i++ ) {
		HashCrackerUtils::IncreasePassword(password, sizeof(password), testAlphabet);
		std::cout << "New password: \"" << password << "\"" << std::endl;
	}
}


void EnqueueDequeue() {
	std::deque<CPasswordChunk> fifo;
	CPasswordChunk chunk;

	fifo.clear();
	
	std::cout << "Queuing 00000aa --> 00000**" << std::endl;
	chunk.Reset();
	chunk.SetPasswordRange( "00000aa", "00000**" );
	fifo.push_back( chunk );

	std::cout << "Queuing 00001aa --> 00001**" << std::endl;
	chunk.Reset();
	chunk.SetPasswordRange( "00001aa", "00001**" );
	fifo.push_back( chunk );

	std::cout << "Element count in FIFO: " << fifo.size() << std::endl;
	while (fifo.size() > 0)
	{
		chunk.Reset();
		chunk = fifo.front();
		fifo.pop_front();
		std::cout << "Poped element: password range [" << chunk.GetPasswordBegin() << ", " << chunk.GetPasswordEnd() << "]" << std::endl;
	}
	std::cout << "Element count in FIFO: " << fifo.size() << std::endl;
}

/*  TEST ZONE  */
/* Fonction pour le thread du magasin. */
//int stock = 0;
const int INITIAL_STOCK = 5;
//pthread_mutex_t mutex_stock = PTHREAD_MUTEX_INITIALIZER;

/*Structure stockant les informations des threads clients et du magasin. */
typedef struct
{
	int stock;

	//pthread_t thread_store;
	//pthread_t thread_clients[NB_CLIENTS];

	pthread_mutex_t mutex_stock;
}
store_t;

static store_t store;
TrueMutex tm = TrueMutex();


/* Fonction pour tirer un nombre au sort entre 0 et max. */
static int get_random(int max)
{
	double val;

	val = (double)max * rand();
	val = val / (RAND_MAX + 1.0);

	return ((int)val);
}


static void * fn_store(void * p_data)
{
	static int cpt = 0;
	static const int maxCpt = 1000;

	while (/*cpt < maxCpt*/1)
	{
		/* Debut de la zone protegee. */
		//pthread_mutex_lock(&store.mutex_stock);
		tm.Lock();

		if (store.stock <= 0)
		{
			store.stock = INITIAL_STOCK;
			printf("Remplissage du stock de %d articles !\n", store.stock);
		}

		/* Fin de la zone protegee. */
		//pthread_mutex_unlock(&store.mutex_stock);
		tm.Unlock();

		cpt++;
	}

	return NULL;
}


/* Fonction pour les threads des clients. */
static void * fn_clients(void * p_data)
{
	int nb = reinterpret_cast<int>(p_data);
	static int cpt = 0;
	static const int maxCpt = 300;

	while (/*cpt < maxCpt*/1)
	{
		int val = get_random(6);

		Sleep(get_random(3));

		/* Debut de la zone protegee. */
		//pthread_mutex_lock(&store.mutex_stock);
		tm.Lock();

		store.stock = store.stock - val;
		printf(
			"Client %d prend %d du stock, reste %d en stock !\n",
			nb, val, store.stock
			);

		/* Fin de la zone protegee. */
		//pthread_mutex_unlock(&store.mutex_stock);
		tm.Unlock();

		++cpt;
	}

	return NULL;
}

int testMain()
{
	int i = 0;
	int ret = 0;
	const int NB_CLIENTS = 3;
	pthread_t thread_store;
	pthread_t thread_clients[NB_CLIENTS];


	tm.Init();

	store.stock = INITIAL_STOCK;
	//store.mutex_stock = PTHREAD_MUTEX_INITIALIZER;

	/* Creation du thread du magasin. */
	printf("Creation du thread du magasin !\n");
	ret = pthread_create(
		&thread_store, NULL,
		fn_store, NULL
		);

	/* Creation des threads des clients si celui du magasin a reussi. */
	if (!ret)
	{
		printf("Creation des threads clients !\n");
		for (i = 0; i < NB_CLIENTS; i++)
		{
			ret = pthread_create(
				&thread_clients[i], NULL,
				fn_clients, (void *)i
				);

			if (ret)
			{
				//fprintf(stderr, "%s", strerror(ret));
				std::cerr << "Erreur" << ret << std::endl;
			}
		}
	}
	else
	{
		//fprintf(stderr, "%s", strerror(ret));
		std::cerr << "Erreur" << ret << std::endl;
	}


	/* Attente de la fin des threads. */
	i = 0;
	for (i = 0; i < NB_CLIENTS; i++)
	{
		pthread_join(thread_clients[i], NULL);
	}
	pthread_join(thread_store, NULL);


	std::cin.get();
	return EXIT_SUCCESS;
}
/* END TEST ZONE */

int main( int argc, const char *argv[] ) {
	//std::cout << "** Welcome to this project skeleton." << std::endl;
	//std::cout << "This is where you need to code the hash cracker." << std::endl;
	//std::cout << std::endl;

	////ExtractCommandLine( argc, argv );
	//GeneratePasswords();
	////EnqueueDequeue();

	//std::cout << std::endl;
	//std::cout << "** Goodbye" << std::endl;
	//std::cin.get();
	//return EXIT_SUCCESS;

	testMain();
}
