/*
HomeWork Number 5:
Win32 Producer and Consumer Problem
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

#define SLOTS 16

///////////////////
// Gloval Variables
///////////////////
int buffer[SLOTS];
int counter;

int totalProducedSoFar;

int numOfProducers;
int numOfConsumers;

int producedInThread;
int consumedInThread;

////////////////////////////////////////
// The Global Thread Control Variables
//////////////////////////////////////
pthread_mutex_t globalMutex;
sem_t fullSemaphore;
sem_t emptySemaphore;

struct ThreadInfo
{
	int ID;
	int count;
}ThreadInfo;

void*  consumer(void* param); // This is the Comsumer Function
void*  producer(void* param); // This is the Producer Function

// Modify Some more...

int main(int argc, char* args[])
{
	if (argc != 4)
	{
		perror("Error This is The Wrong Number Of Arguments.... Please Try Again!");
		exit(0);
	}

	struct ThreadInfo * info;
	pthread_t *producers;
	pthread_t *consumers;

	int i; // Initialize I early ON

	counter = 0; // Initialze the Global Counter to Zero.


	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Getting The ARGS and Placing Them into the corresponding Variables. Must Also Go up in power of 2
	////////////////////////////////////////////////////////////////////////////////////////////////////
	numOfProducers = pow(2, atoi(args[1]));
	numOfConsumers = pow(2, atoi(args[2]));
	totalProducedSoFar = pow(2, atoi(args[3]));


	// Immediately After getting the size, I set the Producer and consumer handles.
	// Also allocate space for my info struct. ThreadInfo
	producers = (pthread_t*) malloc (numOfProducers* sizeof(pthread_t));
	consumers = (pthread_t*) malloc (numOfConsumers* sizeof(pthread_t));
	info = (struct ThreadInfo*) malloc (numOfProducers * sizeof(struct ThreadInfo));
	producedInThread = totalProducedSoFar / numOfProducers;
	consumedInThread = totalProducedSoFar / numOfConsumers;


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Now I will loop Through Each of the Number Of Producers. But First Will Create the Semaphores
	////////////////////////////////////////////////////////////////////////////////////////////////

	if( pthread_mutex_init(&globalMutex,NULL) != 0)
	{
		printf("Lock Semaphore Failed To Instantiate, Now Exiting The Program");
		exit(4);
	}  // This is creating the Global Mutex
	sem_init(&fullSemaphore,0,0); // This is creating The Full Semaphore
	sem_init(&emptySemaphore,0,SLOTS); // This is creating the Empty Semaphore


	for (i = 0; i < numOfProducers; i++)
	{
		info[i].ID = i;
		info[i].count = producedInThread;
		pthread_create(&producers[i],NULL,&producer,&info[i]);
		//producers[i] = CreateThread(NULL, 0, producer, &info[i], 0, &producerThread); // This Function Will Call thread.
	}

	///////////////////////////////////////////////////////////
	// For Each Number of the Consumers I will Create a Thread
	//////////////////////////////////////////////////////////
	for (i = 0; i < numOfConsumers; i++)
	{
		pthread_create(&consumers[i],NULL,&consumer,&consumedInThread);
		//consumers[i] = CreateThread(NULL, 0, consumer, &consumedInThread, 0, &consumerThread);
	}


	/////////////// Waiting for the threads to finish //////////////////
	for(i = 0; i < numOfProducers; i++){pthread_join(producers[i],NULL);}
	for(i = 0; i < numOfConsumers; i++){pthread_join(consumers[i],NULL);}

	printf("Total Items Produced: %i\n The Total Number of Producers: %i\nThe Total Number of Consumers: %i\n", totalProducedSoFar, numOfProducers, numOfConsumers);

	/////// Closing The Thread Control Vars //////
	pthread_mutex_destroy(&globalMutex);
	sem_destroy(&fullSemaphore);
	sem_destroy(&emptySemaphore);

	///////// Freeing Up memory that was allocated ///////
	free(info);
	free(producers);
	free(consumers);

	return 0;
}
void* producer(void* param)
{
	// Variable Declarations
	int item,i;
	struct ThreadInfo * info = (struct ThreadInfo*)  param;

	
	for (i = 0; i < info->count; i++)
	{
		item = info->ID * info->count + i; // The Item That is being Consumed, Is added by the Thread ID and multiplied by the Count.
		sem_wait(&emptySemaphore);
		pthread_mutex_lock(&globalMutex);


		if (counter < SLOTS)
		{
			buffer[counter] = item;
			printf("Item Produced: %i at the Thread: %d\n", item, info->ID);

		}
		else
		{
			perror("Error Occured In the program... Now Exiting\n");
			exit(2);
		}
		counter++;

		pthread_mutex_unlock(&globalMutex);
		sem_post(&fullSemaphore);
	
	}
	pthread_exit(0); // Like the Return Statment but returns the thread.
}


void* consumer(void* param)
{	

	int i; // Declating My 'i' Outside of the Loop

	int numberOfConsumers = *(int *)param;

	////////////////////////////////////////////
	// Will Loop Until the Number Of Consumers
	/////////////////////////////////////////

	for (i = 0; i < numberOfConsumers; i++)
	{
		sem_wait(&fullSemaphore);
		pthread_mutex_lock(&globalMutex);

		if (counter > 0)
		{
			counter--;
			printf("Item Consumed: %i At the Ccounter: %i\n", buffer[counter], counter);
			buffer[counter] = 0;
		}
		else
		{
			perror("Error Occured In the Program...Now Exiting\n");
			exit(3);
		}

		pthread_mutex_unlock(&globalMutex);
		sem_post(&emptySemaphore);
	}

	pthread_exit(0); // This Exits The Thread...
} 