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
    {
      error("You need to provide at least 2 arguments for this program to run");
    }
    
    int fdOpen = open(argv[1], O_RDONLY);
    if (fdOpen == -1)
    {
       error("Cannot open file for reading");
    }
    
    int fdWrite = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
    if (fdWrite == -1)
    {
      error("Cannot open file for writing");
    }
    
     off_t size = lseek(fdOpen, 0, SEEK_END); // seek to end of file
     
     if(size %2 == 0)
     {
        write(fdWrite, "even", 4);
     }
     else 
     {
        write(fdWrite, "odd", 3);
     }
     
     close(fdOpen);
     close(fdWrite);
     
    return 0;
}

