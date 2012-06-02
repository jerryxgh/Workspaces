#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEWFILE (O_WRONLY|O_CREAT|O_TRUNC)

int main(void)
{
    char buf1[]={"abcdefghij"};
    char buf2[]={"1234567890"};
    int fd;
    int length;
    
    if(-1==(fd=open("test.hole",NEWFILE,0600)))
    {
        printf("ERROR,OPEN WRITE FILE FAILED: \n");
        exit(255);
    }
    
    length=strlen(buf1);
    if(write(fd,buf1,length)!=length)
    {
        printf("ERROR,OPEN WRITE FILE FAILED: \n");
        exit(255);
    }
    
    
    if(lseek(fd,50,SEEK_SET)==-1)
    {
        printf("ERROR,LSEEK FAILED: \n");
        exit(255);
    }
    
    length=strlen(buf2);
    if(write(fd,buf2,length)!=length)
    {
        printf("ERROR,OPEN WRITE FILE FAILED: \n");
        exit(255);
    }
    return 0;
}

