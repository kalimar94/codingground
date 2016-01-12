#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void error(const char* errorText)
{
    fprintf(stderr, errorText);
    exit(1);
}

int main(int argc, char** argv)
{
    if(argc < 3)
      error("You need to provide at least 2 arguments for this program to run \n");

    int fdWrite = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
    if (fdWrite == -1)
    {
       error("Cannot open file for writing \n");
    }

    // Prepare pipes:
    int pipes[2];
    pipe(pipes); // Create the pipes
    pid_t currentPid;
    int Child = 0;
    
    if((currentPid = fork()) == -1) 
        error("Error forking the process \n");
    
    if(currentPid == Child)
    {
        dup2 (pipes[1], 1); // pipe[1] should replace standart output
        close(pipes[0]); // Child will not read anything
        close(pipes[1]); // Standart output is now pipe[1]
        execlp("wc", "wc", "-c", argv[1], 0);
    }
    else
    {  
        close(pipes[1]); // Parrent will not write anything

        // Read the result from wc cmd:
        char buffer[1024];
        int charCount;
        read(pipes[0], &buffer, sizeof(buffer));
        sscanf(buffer, "%d", &charCount);
        
        // Write the result to a file
        if(charCount == 0)
        {
           error("input file does not exist or is empty!");
        }
        else if (charCount % 2 == 0)
        {
           write(fdWrite, "even", 4);
        }
        else
        {
          write(fdWrite, "odd", 3);
        }
        
        close(fdWrite);
        
        // Debug info:
        printf("Number of chars: %d \n", charCount);
    }
    
    return 0;
}

