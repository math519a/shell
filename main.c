#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>

int main() {
    //First of all, the user of the shell must know from which directory the shell is running from
    char currentWorkingDirectory[64];
    getcwd(currentWorkingDirectory, sizeof(currentWorkingDirectory));

    //Allocate an adequate amount of memory to store the input string, 512 is just an arbitrary number
    char* userCommandInput = malloc(sizeof(char) * 512);

    while ( 1 ) {
        printf("%s# ", currentWorkingDirectory);

        //Read the input and store it in a string
        fgets(userCommandInput, 512, stdin);

        //If the input string is not null escaped, this will handle it
        char *charIdx;
        if ((charIdx=strchr(userCommandInput, '\n')) != NULL)
            *charIdx = '\0';


        int status = -1;

        //Copy the string for manipulation
        char* inputCpy = malloc(sizeof(char) * (strlen(userCommandInput) + 1));
        strcpy(inputCpy, userCommandInput);

        char * fullCommand = NULL;
        char ** fullCommandArgumentVector = malloc(sizeof(char) * 512 );

        //Using space as a delimiter to tokenize/split up the input string
        char * pch = strtok (inputCpy, " ");
        int caIndex = 0;
        while (pch != NULL)
        {
            if (fullCommand == NULL) {
                //The command itself should be at index [0]
                fullCommand = pch;
            } else {
                *(fullCommandArgumentVector + caIndex++) = pch;
                *(fullCommandArgumentVector + caIndex) = NULL;
            }

            pch = strtok (NULL, " ");
        }

        //Felt the need to implement these two simple command myself because it's essential to navigate a shell

        //quit command
        if (strcmp("quit", fullCommand) == 0) {
            //A status of 1 means the program will terminate at the end of the loop
            status = 1;
        }

        //change directory command
        if (strcmp("cd", fullCommand) == 0) {
            //Use the arguments as destination
            chdir(*fullCommandArgumentVector);

            //For some reason you have to get the working directory after setting it for it to update
            getcwd(currentWorkingDirectory, sizeof(currentWorkingDirectory));
        }

        //A status of -1 means that the shell found a system call
        if (status == -1) {
            char *commandWithWorkingDir = malloc(sizeof(currentWorkingDirectory) + sizeof(userCommandInput) + 5);
            sprintf(commandWithWorkingDir, "cd %s && %s", currentWorkingDirectory, userCommandInput);
            system(commandWithWorkingDir);
        }

        free(fullCommandArgumentVector);
        if (status == 1) break;
    }

    free(userCommandInput);
    return 0;
}

