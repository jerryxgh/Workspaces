#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#define NEWFILE (O_WRONLY|O_CREAT|O_TRUNC)
#define BUFSIZE 1024
#define size 1

int my_read(int fd,char *buf,size_t count);
int my_write(int fd,char *bur,size_t count);
static char read_buf[BUFSIZE];
static int read_offset=BUFSIZE;
static int read_max=BUFSIZE;
static char write_buf[BUFSIZE];
static int write_offset=0;

int main(void)
{
    char infile[ ]={"test.dat"};
    char outfile[ ]={"backup.dat"};
    char buf[size];
    int infd,outfd,count;
    if((infd=open(infile,O_RDONLY))==-1)
    {
        printf("ERROR,OPEN READ FILE FAILED: \n",sys_errlist[errno]);
        exit(255);
    }
    if((outfd=open(outfile,NEWFILE,0600))==-1)
    {
        printf("ERROR,OPEN WRITE FILE FAILED: \n",sys_errlist[errno]);
        exit(255);
    }
    while(count=read(infd,buf,sizeof(buf))>0)
    {
        if(write(outfd,buf,count)!=count)
        {
            printf("ERROR,WRITE FILE FAILED: \n",sys_errlist[errno]);
            exit(255);
        }
    }
    if(count==-1)
    {
        printf("ERROR,READ FILE FAILED: \n",sys_errlist[errno]);
        exit(255);
    }
    close(infd);
    if(write_offset>0)
    {
        write(outfd,write_buf,write_offset);
        write_offset=0;
    }
    close(outfd);
}

int my_read(int fd,char *buf,size_t count)
{
    int i;
    for(i=0; i<count; ++i)
    {
        if(read_offset==read_max)
        {
            read_offset=0;
            read_max=read(fd,read_buf,sizeof(read_buf));
            if(!read_max)
                return i;
        }

        *buf++=read_buf[read_offset++];
    }
    return i;
}

int my_write(int fd,char *buf,size_t count)
{
    int i,n;
    for(i=0; i<count; ++i)
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
    return -1;
}
