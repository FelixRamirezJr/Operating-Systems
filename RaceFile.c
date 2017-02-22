
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <thr\threads.h>
#include <Windows.h>

#define BUFFMEM 65536
#define ASCII 128
#define NUMTHREADS 8
int ascArray[ASCII][2]; // Global 2 Dimensional Array That corresponds to the Ascii Count
int globalArray[ASCII];

void instantiateArray();
void DisplayAscii();

typedef struct buffedStruct
{
	int bytes = 0;
	int threadIdentification = 0;
	int threadCount = 0;
	char * buffer;
};

//////////////////////////////////////////////////////
// Multi Threaded Function That Will Count The Stuff
////////////////////////////////////////////////////
DWORD WINAPI Counting(LPVOID Param) // LPVOID is like void* basically creating Generics...
{
	// Variables
	buffedStruct * bs;
	char * buffer;
	int bytes;
	int threadID;
	int totalThreads;
	int start = 0, end = 0;
	int i = 0;
	int temp = 0;

	// Logic

	bs = (buffedStruct *)Param;
	buffer = bs->buffer;
	bytes = bs->bytes;
	threadID = bs->threadIdentification;
	totalThreads = bs->threadCount;

	start = (threadID - 1) * (bytes / totalThreads); 


	if (threadID < NUMTHREADS)
	{
		end = start + (bytes / totalThreads) - 1;
	}
	else 
	{
		
		end = bytes;
	}

	// Here we loop through the thing... And we are going to look for the N
	for (i = start; i < end + 1; i++)
	{
		temp = buffer[i];
		if (buffer[i] >= 0  && buffer[i] <= 127)
		{
			ascArray[buffer[i]][1]++;
			globalArray[i] = ascArray[i][1];
		}
	}

	
	// Calculate The Start and End Point of the Partition...

	return 0;
}


int main(int argc, char * args[])
{
	DWORD * ThreadIdArray;
	HANDLE * ThreadHandleArray;
	buffedStruct * bs;
	HANDLE file;
	DWORD err;
	char buffer[BUFFMEM];
	DWORD currBytes;
	int totalOccurences = 0; // The total Number of Unique Occurences
	int i = 0;

	instantiateArray();

	if (argc != 2){
		perror("Wrong Number of Command Arguements Entered. Please Try again!\n");
	}

	// Opening the File that was passed in.
	file = CreateFile(args[1], GENERIC_ALL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	err = GetLastError();
	if (err == ERROR_FILE_NOT_FOUND)
	{
		perror("There was an error opening the file\n");
		CloseHandle(file);
		exit(0);
	}

		ReadFile(file, buffer, BUFFMEM, &currBytes, NULL);

		printf("This is the Current Bytes Read: %d\n", currBytes);

		ThreadHandleArray = (HANDLE*)malloc(NUMTHREADS* sizeof(HANDLE)); // Creating a Thread Handle Accounting to the number of Threads...
		ThreadIdArray = (DWORD*)malloc(NUMTHREADS * sizeof(HANDLE)); // Creating an array for Thread ID's according to the Number of Threads..

		bs = (buffedStruct *)malloc(NUMTHREADS * sizeof(buffedStruct)); // Allocating Memory for the Buff Struct...

		// Instantiating the struct
		for (i = 0; i < NUMTHREADS; i++)
		{
			bs[i].buffer = buffer;
			bs[i].bytes = currBytes;
			bs[i].threadIdentification = i + 1;
			bs[i].threadCount = NUMTHREADS;
		}

		// Creating the threads... and Calling the Function...

		for (i = 0; i < NUMTHREADS; i++)
		{
			if (ThreadHandleArray != NULL)
			{
				ThreadHandleArray[i] = CreateThread(NULL, 0, Counting, &bs[i], 0, &ThreadIdArray[i]);
			}
		}

		// Waiting For All Threads
		for (i = 0; i < NUMTHREADS; i++)
		{
			if (ThreadHandleArray != NULL)
			{
				WaitForSingleObject(ThreadHandleArray[i], INFINITE);
			}
		}

		// Close all Thread Handles...

		for (i = 0; i < NUMTHREADS; i++)
		{
			if (ThreadHandleArray[i] != NULL)
			{
				CloseHandle(ThreadHandleArray[i]);
			}
		}
		CloseHandle(file);


		DisplayAscii(); 

	return 0;
}


//////////////////////////////////////////////////////
// This function will instantiate the array
// So it can be used correctly throughout
// the entire program
//////////////////////////////////////////////////////
void instantiateArray()
{
	int i = 0;
	for ( i = 0; i < 128; i++)
	{
		ascArray[i][0] = i; // Setting up the First Column to the ASCII values
		ascArray[i][1] = 0; // Setting up the Second Column to the Count of The Ascii's Purposely instantiating them to ZERO!
		globalArray[i] = 0;
	}
}

void DisplayAscii()
{
	int i = 0;
	for (i = 0; i < 128; i++)
	{
		//printf("This ASCII VAL : %d   This is Count: %d\n", ascArray[i][0], ascArray[i][1]);
		if (ascArray[i][1] >= 0)
		{
			if (i >= 0 && i <= 32)
			{
				//print The HEXA DECIMAL.. WILL NOT SHOW UP CORRECTLY
				printf("%d Occurences of Hexa Value 0X%02x\n", globalArray[i], ascArray[i][0]); // Print HexaValue for THE NON SHOWING
			}
			else
			{
				printf("%d Occurences of %c\n", globalArray[i], ascArray[i][0]); // Print the Symbol
			}
		}

	}
}	
