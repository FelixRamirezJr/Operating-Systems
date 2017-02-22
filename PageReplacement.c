/*
LRU and FIF0 algorithms
By Felix Ramirez
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX = 999999

// Global Variables
int * pageTable;
int * frame;
int pageFaults;


struct BuffData
{
	char * str;
	int hexaDecimalVal;
};

struct LRU
{
	int num;
	int priority;
};

struct LRU * lru;
int usedTime = 0;

// Functions Used In my program
void FIFO(int item, int length);
void LRUFUN(int item, int length);


int main(int argc, char * args[])
{
	// Variables that I will be using..
	struct BuffData * buffer;
	int pageSize;
	int logicalMemorySize;
	char * algo;
	int numOfFrames;
	char string[60];
	int random;
	int pageReferences[100];
	int i = 0;
	int j = 0;
	int x = 0;
	int fillFrameCount;
	int number = 0;

	// 2^page size = pageSize
	// 2^logicalMemory = Logical Memory

	if (argc < 5)
	{
		perror("Invalid Number of Arguments, Please Try Again!");
		exit(0);
	}

	// Getting All the Command Argument Paramets Necessary For this assignment.
	pageSize = atoi(args[1]);
	pageSize = (int)pow(2, pageSize);

	logicalMemorySize = atoi(args[2]);
	logicalMemorySize = (int)pow(2, logicalMemorySize);

	algo = args[3];
	numOfFrames = atoi(args[4]);

	//////// Allocating Memory For the Page Tables /////
	pageTable = (int*)malloc(sizeof(int) * pageSize);
	frame = (int*)malloc(sizeof(int) * numOfFrames);
	//int *arr = malloc (sizeof (int) * n);

	// Creating the 100 random page Sizes..

		printf("This is the Page Size: %d\n", pageSize);
		printf("This is the logicalMemorySize: %d", logicalMemorySize);
		printf("\nThis is the Algorithm that was chosen: %s", algo);
		printf("\nThis is the Number of Frames: %d", numOfFrames);

		
		// Going to Read into the Page Table and Do one of the sorting algorithms.
		i = 0;
		pageFaults = 0;

		//// GOING TO KEEP READING INPUT AND CONVERTING THE HEXADECIMAL TO A REGULAR VALUE.
		do
		{
			printf("\n(Press Enter To Enter The Second Part oF the Program)\n" 
				"Enter a Hexadecimal Line 0x ");
			fgets(string, 60, stdin);

			if (string[0] == '\n' || string[0] == '\0')
			{
				printf("Should Be A new line\n");
				break;
			}
			number = (int) strtol(string, NULL, 16);
			pageTable[i] = number; // Converting The Hexadecimal To Decimal For the Page Table
			
			printf("Input Hex: %s converted into Decimal %d and Added To the Page Table\n", string,pageTable[i]);

			///// This Will Execute The LRU Algorithm
			if (strcmp(algo, "LRU") == 0 || strcmp(algo, "lru") == 0)
			{
				printf("Executing FIFO Algorithm\n");

				for (j = 1; j <= numOfFrames + 1; j++)
				{
					pageFaults = 0;
					frame = malloc((numOfFrames + 1)*sizeof(int));
					for (fillFrameCount = 0; fillFrameCount < j; fillFrameCount++)
					{
						frame[fillFrameCount] = pageTable[fillFrameCount];
					}
					for (x = j; x < pageSize; x++)
					{
						FIFO(pageTable[x], j);
					}
					printf("Page faults: %d, frames:%d\n", pageFaults + 1, j);
				}
			}
			// Below Will Execute the FIFO algorithm
			if (strcmp(algo, "FIFO") == 0 || strcmp(algo, "fifo") == 0 )
			{
				for (int j = 1; j <= numOfFrames; j++)
				{
					//Reset page faults (LRU)
					pageFaults = 0;
					usedTime = 0;
					lru = malloc(j * sizeof(struct LRU));
					for (int iLRU = 0; iLRU < j; iLRU++)
					{
						struct LRU value;
						value.num = pageTable[iLRU];
						value.priority = usedTime;
						lru[iLRU] = value;
					}

					//Fill other pages using LRU 
					for ( x = j; x < 100; x++)
					{
						LRUFUN(pageTable[x], j);
						//increment priority
						usedTime++;
					}
					printf("Page faults: %d, frames:%d\n", pageFaults + 1, j);
				}
			}

			// Should Convert a HexaDecimal To a Decimal.
			i++; // Continusly Updating the Page Table

		} while (1);
		
		///////////////////////////////////////////////////////////////////////////////
		////////////////// SECOND PART OF THE LAB, CREATING THE 100 RANDOM REFERENCES
		//////////////////////////////////////////////////////////////////////////////
		// Manually Chaning the Page Size to 4k F
		numOfFrames = 16;
		printf("Now Creating 100 Random Page References Press Enter To continue!");

		for (i = 0; i < 100; i++)
		{
			//pageReferences[i] = 0;
			random = rand() % 16 + 1;
			if (i > 0) 
			{

				//If reference is the same as previous number,
				//make new reference (no consecutive numbers)
				while (pageTable[i - 1] == random)
				{
					random = rand() % 16 + 1;
				}
			}
			pageTable[i] = random;
		}

		for (i = 0; i < 100; i++)
		{
			printf("\n %d Page Reference: %d \n", i, pageTable[i]);
		}
		///////////////////////////////////////////////////
		//////////////// END OF CREATING RANDOM REFERENCES
		//////////////////////////////////////////////////

		///////////////////////////// FIFO ALGORIGHT WITH 100 ///////////
		/// Prints the Following in increasing order.
		for (j = 1; j <= numOfFrames + 1; j++)
		{
			pageFaults = 0;
			frame = malloc((numOfFrames + 1)*sizeof(int));
			for (fillFrameCount = 0; fillFrameCount < j; fillFrameCount++)
			{
				frame[fillFrameCount] = pageTable[fillFrameCount];
			}
			for (x = j; x < pageSize; x++)
			{
				FIFO(pageTable[x], j);
			}
			printf("FIFO Page faults: %d, frames:%d\n", pageFaults + 1, j);
		}

		/////////////////////////// LRU ALGO WITH 100 RANDOM ////////////////

		for (int j = 1; j <= numOfFrames; j++)
		{
			//Reset page faults (LRU)
			pageFaults = 0;
			usedTime = 0;
			lru = malloc(j * sizeof(struct LRU));
			for (int iLRU = 0; iLRU < j; iLRU++)
			{
				struct LRU value;
				value.num = pageTable[iLRU];
				value.priority = usedTime;
				lru[iLRU] = value;
			}

			//Fill other pages using LRU 
			for (x = j; x < 100; x++)
			{
				LRUFUN(pageTable[x], j);
				//increment priority
				usedTime++;
			}
			printf("LRU Page faults: %d, frames:%d\n", pageFaults + 1, j);
		}

		printf("\n\n");
		system("pause");

		return 0;
	
}

void FIFO(int item, int length){

	for (int i = 0; i < length; i++) {
		//Check frames
		//If page is already in frame, break.
		if (frame[i] == item) {
			break;
		}
		else if (i == length - 1)
		{
			//else if page is not loaded after reading all frames
			//Replace index and shift array
			for (int index = 0; index < length; index++) {
				frame[index] = frame[index + 1];
			}
			//Insert the new page into the last frame.
			frame[length - 1] = item;
			pageFaults++;
		}
	}
}

//Least Recently Used Function
void LRUFUN(int item, int length){

	int leastUsed = 0;
	int current = 0;

	for (int j = 0; j < length; j++) {
		//Check frames
		//If page is already in the frame, break
		if (lru[j].num == item) {
			//Keep track of priority (LRU)
			lru[j].priority = usedTime;
			break;

		}
		else if (j == length - 1){
			//else if page is not loaded after reading all frames   
			//Locate least recently used.
			leastUsed = lru[0].priority;

			//Replace old pages based on priority
			for (int index = 0; index < length; index++) {
				if (lru[index].priority < leastUsed) {
					leastUsed = lru[index].priority;
					current = index;
				}
			}
			//Replace old page with new value and priority
			lru[current].num = item;
			lru[current].priority = usedTime;
			//Increment page fault
			pageFaults++;
		}
	}
}