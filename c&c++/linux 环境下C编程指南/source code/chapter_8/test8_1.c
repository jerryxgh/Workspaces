#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
int write_open(const char *pathname,mode_t mode)
{
    int retval; 
    if((retval=open(pathname,O_WRONLY|O_CREAT|O_TRUNC,mode)==-1))
    {
        printf("ERROR,OPEN FILE FAILED!\n");
        exit(254);
    }
    return retval;
}

int main()
{
    return write_open("test.txt",0600);
    
}
