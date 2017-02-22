//////////////////////////////////////////////////////////////////////////////////////////
// The Following Program Will Replicate a Command Promt/Terminal Using Only Basic Commands
// Created By: Felix Ramirez
// Date: 1/8/2015
// //////////////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS // Getting rid of Any Warning.. Using Visual Studio 2013..
#endif

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#define CMDNAME "Felix's Shell$ "
#define MAXMEM 2048
#define duhleet " \n"
#define ERROROPEN "Could Not Open Output File. Program Exiting\n"
#define PIPEFAIL "Could Not Create Pipe. Program Exiting\n"
#define PROCESSFAIL "Could Not Create Process. Program Exiting\n"

int main(int argc, char * args[])
{

	int count = 1; // Random Counter to be used in the program...
	char command[MAXMEM]; // Creating a variable to place the command that was entered... It is the size 2048 as it was requested...

	if (argc > 1)
	{

		////////
		// EXIT
		////////
		if ((strcmp(args[1], "exit") == 0) || (strcmp(args[1], "EXIT") == 0) || (strcmp(args[1], "QUIT") == 0) || (strcmp(args[1], "quit") == 0))
		{
			exit(0);
			// The user Has decided to exit the program and quit the application...
		}

		///////////////////////////////////////////////
		// Getting The Commands into a character array
		//////////////////////////////////////////////
		strcpy(command, args[1]);
		strcat(command, " ");

		for (int i = 2; i < argc; i++)
		{
			strcat(command, *(args + i));
			strcat(command, " ");
			count++;
		}

		// Change as much as you can past this Point.... Looks like it was taken from the interent????
		STARTUPINFO startInfo; // a struct for Processing The Start Information
		PROCESS_INFORMATION processInfo; // A Struct for Processing the information...

		ZeroMemory(&startInfo, sizeof(startInfo)); //  Calling Zero Memory Function...
		startInfo.cb = sizeof(startInfo); // startInfo.. This is allocating space for the Start Information...
		ZeroMemory(&processInfo, sizeof(processInfo));

		//Go through the process (reading buffer)

		if (!CreateProcess(NULL,	   // No module name (use command line)
			command,					   // Command line
			NULL,					   // Process handle not inheritable
			NULL,					   // Thread handle not inheritable
			TRUE,					   // Set handle inheritance to TRUE
			0,						   // No creation flags
			NULL,					   // Use parent's environment block
			NULL,					   // Use parent's starting directory 
			&startInfo,					   // Pointer to STARTUPINFO structure
			&processInfo))					   // Pointer to PROCESS_INFORMATION structure 
		{
			printf("CreateProcess failed (%d).\n", GetLastError());
			return 0;
		}

		// Wait until child process exits
		WaitForSingleObject(processInfo.hProcess, INFINITE);

		// Close process and thread handles 
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);


	}

	////////////////////////////
	// If Command Is less than 1
	////////////////////////////

	else
	{
		while (1)
		{
			char userInput[MAXMEM];
			char userInputCopy[MAXMEM]; //bufcopy
			char inputOne[MAXMEM + 7]; //input
			char inputTwo[MAXMEM + 7]; //input2
			char cmdINPUT[MAXMEM];
			int i = 0;
			int size = 0;
			int background = 0;
			int pipe = 0;
			char * input;
			char * tokenizer;

			STARTUPINFO startInfo; // si
			STARTUPINFO startInfoTwo; //si2


			PROCESS_INFORMATION processInfo;//pi
			PROCESS_INFORMATION processInfoTwo;//pi2

			HANDLE rightHandle; //   Right Handle
			HANDLE leftHandle;// Left Handle   

			SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };



			// Creating The Zero Memory Factors.
			ZeroMemory(&startInfo, sizeof(startInfo));
			ZeroMemory(&processInfo, sizeof(processInfo));

			ZeroMemory(&startInfoTwo, sizeof(startInfoTwo));
			ZeroMemory(&processInfoTwo, sizeof(processInfoTwo));

			startInfoTwo.cb = sizeof(startInfo);
			startInfo.cb = sizeof(startInfoTwo);

			startInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
			startInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
			startInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
			startInfo.dwFlags = STARTF_USESTDHANDLES;

			printf("%s", CMDNAME);

			// Reading The Input From The User.
			fgets(userInput, MAXMEM, stdin);
			input = strtok(userInput, "\n");
			
			printf("This is the Input: %s    and the Length: %d", input, strlen(input));

			if (strlen(input) > 0) // If the user actually entered something. Wll only run if the 
			{

				if ( (strcmp(input, "EXIT") == 0) || (strcmp(input, "exit") == 0) || (strcmp(input, "quit") == 0) || (strcmp(input, "QUIT") == 0) ) // Checking if the User is trying to quit..
				{
				printf("Thank you for using my Felix's Shell... Hope you had a great time using it\n");
					exit(1); // This is the Second Exit... Which Means it is exiting the Command Prompt...
				}
				
				strcpy(cmdINPUT, "cmd /c ");
				while (input != NULL)
				{ 
				if (strcmp(input, "&") == 0)
				{
					background = 1; // Setting The Background
				}

				else if ((strcmp(input, ">") == 0) || (strcmp(input, "2>") == 0))
				{
					input = strtok(NULL, duhleet); 


					if (strcmp(input, ">") == 0) 
					{
						
						if ((startInfo.hStdOutput = CreateFile(input, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
						{
							perror(ERROROPEN);
							exit(2);
						}
						else
						{
							if ((startInfo.hStdError = CreateFile(input, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
							{
								perror(ERROROPEN);
								exit(3);
							}
						}
					}
				}

					else if ((strcmp(input, "2>>") == 0) || (strcmp(input, ">>") == 0)) // Checks If the Input Is 2>> or >>
					{
						if ((strcmp(input, ">>") == 0))
						{
							
							if ((startInfo.hStdOutput = CreateFile(input, FILE_APPEND_DATA, 0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
							{
								if ((startInfo.hStdOutput = CreateFile(input, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE){
									perror(ERROROPEN);
									exit(4);
								}
							}
						}
						else 
						{
							if ((startInfo.hStdError = CreateFile(input, FILE_APPEND_DATA, 0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
							{
								startInfo.hStdError = CreateFile(input, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
							}

							if (startInfo.hStdError == INVALID_HANDLE_VALUE)
							{
								perror(ERROROPEN);
								exit(5);
							}
						}

					}

					else if ((strcmp(input, "<") == 0))
					{
						if ((startInfo.hStdInput = CreateFile(input, GENERIC_READ, 0, &sa, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
						{
							perror(ERROROPEN);
							exit(6);
						}

					}

					else if ((strcmp(input, "|")) == 0) // The PIPE OPERATOR.
					{
						pipe = 1;
						//get second command
						strcpy(inputTwo, "cmd /c ");
						input = strtok(NULL, duhleet);
						while (input != NULL)
						{
							strcat(inputTwo, input);
							strcat(inputTwo, " ");
							input = strtok(NULL, duhleet);
						}
						//fix suffix (replace last " " with \0)
						size = strlen(inputTwo);
						inputTwo[size - 1] = '\0';
					}
					else
					{
						strcat(cmdINPUT, input);
						strcat(cmdINPUT, " ");
					}
					
				input = strtok(NULL, duhleet);

				}
				size = strlen(cmdINPUT);
				cmdINPUT[size - 1] = '\0';

				if (pipe == 1 || pipe)
				{
					if (!CreatePipe(&rightHandle, &leftHandle, &sa, 0)) // Child Process
					{
						perror(PIPEFAIL);
						exit(11);
					}

					// Doing All The Handles Stuff
					startInfoTwo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
					startInfoTwo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
					startInfoTwo.hStdOutput = leftHandle;
					startInfoTwo.dwFlags = STARTF_USESTDHANDLES;

					if (!CreateProcess(NULL, cmdINPUT, NULL, NULL, TRUE, 0, NULL, NULL, &startInfoTwo, &processInfoTwo))
					{
						perror(PROCESSFAIL);
						exit(12);
					}
				}

				else // If Pipe is Not at one... Then We must create a child Process
				{
					if (!CreateProcess(NULL, cmdINPUT, NULL, NULL, TRUE, 0, NULL, NULL, &startInfo, &processInfo))
					{
						perror(PROCESSFAIL);
						exit(13);
					}
				}

				// Checks If the Pipe is till available, If it is then We Wait for the processes to finish
				if (pipe)
				{ 
					// Then We wait for the Processes to finish
					WaitForSingleObject(processInfo.hProcess, INFINITE);
					WaitForSingleObject(processInfoTwo.hProcess, INFINITE);
				}

				else
				{
					// If there is still a process running in the background, then we Wait for the Object.
					if (!background)
					{
						WaitForSingleObject(processInfo.hProcess, INFINITE);
					}
				}


				//WaitForSingleObject(processInfo.hProcess, INFINITE); // Waitinf for the Single Object... Not sure what this is about.... should check up on it...

				CloseHandle(processInfo.hProcess);
				CloseHandle(processInfo.hThread);

				if (startInfo.hStdInput != GetStdHandle(STD_INPUT_HANDLE))
				{
					CloseHandle(startInfo.hStdInput);
				}

				if (startInfo.hStdError != GetStdHandle(STD_ERROR_HANDLE))
				{
					CloseHandle(startInfo.hStdError);
				}
				if (startInfo.hStdOutput != GetStdHandle(STD_OUTPUT_HANDLE))
				{
					CloseHandle(startInfo.hStdOutput);
				}

				if (pipe)
				{
					CloseHandle(processInfoTwo.hThread);
					CloseHandle(processInfoTwo.hProcess);
					CloseHandle(leftHandle);
					CloseHandle(rightHandle);
				}


				// This is processing the Information...

			} // End of the String Length Surpassing the Thingy...
		}
	} // End Of the Else Statement

	return 0; // End of all the Logic... Hope all this shit fucken works LOL
}
