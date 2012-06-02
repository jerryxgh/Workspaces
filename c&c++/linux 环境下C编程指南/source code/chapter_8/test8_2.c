#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define NEWFILE (O_WRONLY|O_CREAT|O_TRUNC)
#define SIZE 80
#define BUFSIZE 50

int my_write(int fd,char *buf,size_t count);

char write_buf[BUFSIZE];
int write_offset = 0;

int main(void)
{
    int outfile;
    char filename[]={"test.dat"};
    char buffer[SIZE];
    if((outfile=open(filename,NEWFILE,0640))==-1)
    {
        printf("ERROR,OPEN FILE FAILED!\n");
        exit(255);
    }

    gets(buffer);

    while(strcmp(buffer,"quit"))
    {
        if(my_write(outfile,buffer,strlen(buffer))==-1)
        {
            printf("ERROR,WRITE FAILED:\n",sys_errlist[errno]);
            exit(255);
        }
        gets(buffer);
    }
    close(outfile);
    return 0;
}

int my_write(int fd,char *buf,size_t count)
{
    int i,n;
    
    for(i=0; i<count; i++)
    {
        write_buf[write_offset++]=*buf++;

        if(write_offset==BUFSIZE)
        {
            write_offset=0;
            n=write(fd,write_buf,sizeof(write_buf));
            if(n!=BUFSIZE) 
                return -1;
        }
    }
    return 0;
}
