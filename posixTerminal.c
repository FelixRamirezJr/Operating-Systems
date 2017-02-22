/*
Created By Felix Ramirez
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/wait.h>
#include <sys/types.h>

#define CMDNAME "MyShell>>"
#define MAXMEM 2048
#define duhleet " \n"
#define ERROROPEN "Could Not Open Output File. Program Exiting\n"
#define PIPEFAIL "Could Not Create Pipe. Program Exiting\n"
#define PROCESSFAIL "Could Not Create Process. Program Exiting\n"


int main ()
{

while(1){
//prompt user input
    printf(CMDNAME);
//initialize
    int BACKGROUND = 0;
    int status;
    int i = 0;
    int j = 0;

    char userInput[MAXMEM];
    char userInputCopy[MAXMEM]; 
    char * argsOne[MAXMEM];
    char * argsTwo[MAXMEM]; 
    char inputOne[MAXMEM + 7]; 
    int pidOne = 0;
    int tempCount = 0;
    int pidTwo = 0;

    int pipeObject = 0;
    int file = 0;
    int inputFile = 0;
    char * input;
    int count = 0;
    int error = 0;
    int argumentOne = 0; // Creating Variables for the Argument.
    int arguemntTwo = 0; // Creating Variable for the Second Arguemnt.
    int tempc, myargc2 = 0, myargc1 = 0;
    int pid1,pid2;
    int pipeID[2];
    //int status;


    fgets(userInput, MAXMEM, stdin); // Reads The Input and Then gets rid of the New Line

    strncpy(userInputCopy, userInput, MAXMEM); // Make A copy for Later Use.

    input = strtok(userInput, "\n"); // Get Rid of the New line Char


    while(input != NULL)
    {
    if(strcmp(input, "&") == 0)
    {
        BACKGROUND = 1; // Set the Background to 1
    }
    else if(strcmp(input, ">") == 0 || strcmp(input, "2>") == 0)
    {
        if(strcmp(input, "2>") == 0)
        {
            error = 1;
        }

        input = strtok(NULL, duhleet);
        file = open(input, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH); // Tries To create The File and if it fails then exit the program
        if(file == -1)
        {
        perror(ERROROPEN);
        exit(0);
        }

    }
    else if(strcmp(input, ">>") == 0 || strcmp(input, "2>>") == 0)
    {
        if(strcmp(input, "2>>") == 0)
        {
        error = 1; 
        }

        input = strtok(NULL, duhleet);
 
        file = open(input, O_WRONLY|O_APPEND|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH); // Tries to Open a file Again! If It fails Please exit the program
        if(file == -1)
        {
        perror(ERROROPEN);
        exit(1);
        }
    }
    else if(strcmp(input, "<") == 0)
    {
    input = strtok(NULL, duhleet);
    inputFile = open(input, O_RDONLY, NULL); // Reads the Input File

        if(file == -1)
        {
            perror(ERROROPEN);
            exit(0);
        }
    }
    else if(strcmp(input, "|") == 0)
    {
        pipeObject = 1;
        tempCount = myargc1;
        myargc1 = 0;
    }

    else
    {
        myargc1++; // Check this
    }
        input = strtok(NULL, duhleet);
    }
    if(myargc1 != 0)
    {
    //store strings
    if(pipeObject)
    {

    myargc2 = myargc1;
    myargc1 = tempCount;
    }

    char *myargumentsOne[myargc1 + 1];
    char *myarguemtsTwo[myargc2 + 1];
    i = 0; 
    j = 0;
    myargumentsOne[i] = strtok(userInputCopy, duhleet);

    do
    {
        if(BACKGROUND)
        {
        if(strcmp(myargumentsOne[i], "&") == 0)
        {
         myargumentsOne[i] = strtok(NULL, duhleet);
         break;
        }   

        }
    if(file) // If File Already Exists
    {
//skip over file name
        if(strcmp(myargumentsOne[i], ">") == 0 || strcmp(myargumentsOne[i], "2>") == 0)
        {
        strtok(NULL, duhleet);
        myargumentsOne[i] = strtok(NULL, duhleet);
        break;
        }
        else if(strcmp(myargumentsOne[i], ">>") == 0 ||strcmp(myargumentsOne[i], "2>>") == 0)
        {
        strtok(NULL, duhleet);
        myargumentsOne[i] = strtok(NULL, duhleet);
        break;
        }
    }
    if(inputFile)
    {
        if(strcmp(myargumentsOne[i], "<") == 0)
        {
        strtok(NULL, duhleet);
        myargumentsOne[i] = strtok(NULL, duhleet);
        break;
        }
    }
    if(pipeObject)
    {
        if(strcmp(myargumentsOne[i], "|") == 0)
        {
    //store second command
         do
        {
         myarguemtsTwo[j] = strtok(NULL, duhleet);
        }while(myarguemtsTwo[j++] != NULL);

        myargumentsOne[i] = myarguemtsTwo[--j];
         break;
         }
    } // End of Pipe Line
    myargumentsOne[++i] = strtok(NULL, duhleet);
    }while(myargumentsOne[i] != NULL);
//create new process
    if(pipeObject)
    {//with pipe
        if ((status = pipe(pipeID)) == -1) 
        {
        perror("Pipe Went Wrong");
        exit(12);
        }

        if ((pid1 = fork()) == -1) 
        {
            perror("Forking Went Bad");
            exit(13);
        }

        if (pid1 == 0) 
        {// first child
            close(pipeID[1]);
            close(0);
            dup(pipeID[0]);
//execute
        if(execvp(myarguemtsTwo[0], myarguemtsTwo) < 0)
        {
            if(strcmp(myarguemtsTwo[0], "exit") != 0)
            {
             perror("Command Not Found");
            }
        exit(1);
        }
        close(pipeID[0]);
        }
//parent
    if ((pid2 = fork()) == -1) 
    {
        perror("Bad fork");
        exit(-1);
    }

    if (pid2 == 0) {
    //second child
    close(pipeID[0]);
    close(1);
    dup(pipeID[1]);
//execute
    if(execvp(myargumentsOne[0], myargumentsOne) < 0)
    {
        if(strcmp(myargumentsOne[0], "exit") != 0)
        {
          perror("Command Not Found");
        }
        exit(1);
    }
        close(pipeID[1]);
    }
//parent
    close(pipeID[0]);
    close(pipeID[1]);
    while(wait(&status) != pid2);
    while(wait(&status) != pid1);
    }else
    {//whitout pipe
    if ((pid1 = fork()) == -1) {
perror("Bad fork");
exit(-1);
}
if(pid1 == 0) {
//child proccess
if(file){
close(1);
dup(file);
}
if(inputFile){
close(0);
dup(inputFile);
}
if(error)
{
dup2(file,2);
}
//execute
if(execvp(myargumentsOne[0], myargumentsOne) < 0)
{
    if(strcmp(myargumentsOne[0], "exit") != 0)
    {
    perror("Unkown COmmand");    
    }
    exit(1);
}
if(file || error)
{
close(file);
}

if(inputFile)
{
    close(inputFile);
}

}
else
{
//parent process
//wait for child process unless specified
if(!BACKGROUND)
{
    while(wait(&status) != pid1);
}

}
}

if(strcmp(myargumentsOne[0], "exit") == 0)
{
exit(0);
}

}
}//end loop
return 0;
}
