/*
HomeWork Number 5:
Win32 Producer and Consumer Problem
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

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
HANDLE globalMutex;
HANDLE fullSemaphore;
HANDLE emptySemaphore;

struct ThreadInfo
{
	int ID;
	int count;
}ThreadInfo;

DWORD WINAPI consumer(LPVOID param); // This is the Comsumer Function
DWORD WINAPI producer(LPVOID param); // This is the Producer Function

// Modify Some more...

int main(int argc, char* args[]){
	struct ThreadInfo * info;
	HANDLE *producers;
	HANDLE *consumers;
	DWORD producerThread;
	DWORD consumerThread;
	int i; // Initialize I early ON

	counter = 0; // Initialze the Global Counter to Zero.

	if (argc != 4)
	{
		perror("Error This is The Wrong Number Of Arguments.... Please Try Again!");
		exit(0);
	}// check for correct inputs file and datas


	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Getting The ARGS and Placing Them into the corresponding Variables. Must Also Go up in power of 2
	////////////////////////////////////////////////////////////////////////////////////////////////////
	numOfProducers = pow(2, atoi(args[1]));
	numOfConsumers = pow(2, atoi(args[2]));
	totalProducedSoFar = pow(2, atoi(args[3]));


	// Immediately After getting the size, I set the Producer and consumer handles.
	// Also allocate space for my info struct. ThreadInfo
	producers = (HANDLE *)malloc(numOfProducers * sizeof(HANDLE));
	consumers = (HANDLE *)malloc(numOfConsumers * sizeof(HANDLE));
	info = (struct ThreadInfo *) malloc(numOfProducers * sizeof(struct ThreadInfo));


	producedInThread = totalProducedSoFar / numOfProducers;
	consumedInThread = totalProducedSoFar / numOfConsumers;


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Now I will loop Through Each of the Number Of Producers. But First Will Create the Semaphores
	////////////////////////////////////////////////////////////////////////////////////////////////

	globalMutex = CreateSemaphore(NULL, 1, 1, NULL); // This would be the Semaphore in charhe of Locking
	fullSemaphore = CreateSemaphore(NULL, 0, SLOTS, NULL); // The "full Semaphore"
	emptySemaphore = CreateSemaphore(NULL, SLOTS, SLOTS, NULL);// The "empty Semaphore"


	for (i = 0; i < numOfProducers; i++)
	{
		info[i].ID = i;
		info[i].count = producedInThread;
		producers[i] = CreateThread(NULL, 0, producer, &info[i], 0, &producerThread); // This Function Will Call thread.
	}

	///////////////////////////////////////////////////////////
	// For Each Number of the Consumers I will Create a Thread
	//////////////////////////////////////////////////////////
	for (i = 0; i < numOfConsumers; i++)
	{
		consumers[i] = CreateThread(NULL, 0, consumer, &consumedInThread, 0, &consumerThread);
	}

	/////////////// Waiting for the threads to finish //////////////////
	WaitForMultipleObjects(numOfProducers, producers, TRUE, INFINITE);
	WaitForMultipleObjects(numOfConsumers, consumers, TRUE, INFINITE);

	printf("Total Items Produced: %i\n The Total Number of Producers: %i\nThe Total Number of Consumers: %i\n", totalProducedSoFar, numOfProducers, numOfConsumers);

	/////// Closing The Thread Control Vars //////
	CloseHandle(globalMutex);
	CloseHandle(fullSemaphore);
	CloseHandle(emptySemaphore);

	///////// Freeing Up memory that was allocated ///////
	free(info);
	free(producers);
	free(consumers);

	return 0;
}
DWORD WINAPI producer(LPVOID param)
{
	// Variable Declarations
	struct ThreadInfo * info = (struct ThreadInfo*)  param;
	int item;
	int i;

	
	for (i = 0; i < info->count; i++)
	{
		item = info->ID * info->count + i; // The Item That is being Consumed, Is added by the Thread ID and multiplied by the Count.
		WaitForSingleObject(emptySemaphore, INFINITE);
		WaitForSingleObject(globalMutex, INFINITE);


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

		ReleaseSemaphore(globalMutex, 1, NULL);
		ReleaseSemaphore(fullSemaphore, 1, NULL);
	}
	return 0;
}


DWORD WINAPI consumer(LPVOID param)
{	

	int i; // Declating My 'i' Outside of the Loop

	int numberOfConsumers = *(int *)param;

	////////////////////////////////////////////
	// Will Loop Until the Number Of Consumers
	/////////////////////////////////////////

	for (i = 0; i < numberOfConsumers; i++)
	{
		WaitForSingleObject(fullSemaphore, INFINITE); // Waiting For Synch. So we do not have any deadlock.
		WaitForSingleObject(globalMutex, INFINITE);

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

		ReleaseSemaphore(globalMutex, 1, NULL);
		ReleaseSemaphore(emptySemaphore, 1, NULL);
	}

	return 0;
} 