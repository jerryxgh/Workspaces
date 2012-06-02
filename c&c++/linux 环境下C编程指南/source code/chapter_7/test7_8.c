#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>

void parent(char *argv[]);
void child(char *argv[]);
int write_buffer(int fd,const void *buf,int count);
int read_buffer(int fd,void *buf,int count);
int readnlstring(int socket,char *buf,int maxlen,char delim);

int main(int argc,char *argv[ ])
{
    if(argc<2)
        parent(argv);
    else
        child(argv);
    return 0;


}
void parent(char *argv[])
{
    char buffer[100];
    int fd;
    close(0);
    if(mkfifo("my-fifo",0600)==-1)
    {
        printf("error,failed to creat my-fifo!\n");
        exit(254);
    }
    printf("the server is listening on my-fifo.\n");
    if((fd=open("my-fifo",O_RDONLY))==-1)
    {
        printf("error,failed to open my-fifo!\n");
        exit(254);
    }
    printf("Client has connected.\n");

    while(1)
    {
        int ret;
        if((ret=readnlstring(fd,buffer,sizeof(buffer),'\n'))>0)
        {
            *(buffer+ret) = 0x00;
            printf("received message:%s\n",buffer);
        }
        else{

            break;
        }
    }

    printf("No more data; parent exiting.\n");
    if(close(fd)==-1)
    {
        printf("error,close failed!");
        exit(254);
    }
    unlink("my-fifo");
}
void child(char *argv[])
{
    int fd;
    char buffer[100];
    if((fd=open("my-fifo",O_WRONLY))==-1)
    {
        printf("error,failed to open my-fifo!\n");
        exit(254);
    }
    printf("The client is ready.Enter messages,or Ctrl+D when done.\n");

    while(fgets(buffer,sizeof(buffer),stdin)!=NULL)
    {
        write_buffer(fd,buffer,strlen(buffer));
    }
    printf("client exiting.\n");
    if(close(fd)==-1)
        printf("error,close failed!");
    exit(254);
}

int write_buffer(int fd,const void *buf,int count)
{
    const void *pts=buf;
    int status=0,n;
    if(count<0)
        return(-1);
    while(status!=count)
    {
        if((n=write(fd,pts+status,count-status))==-1)
        {
            printf("error,failed to write!\n");
            exit(254);
        }else if(n<0)
        {
            return (n);
        }
        status+=n;
    }

    return(status);
}
int read_buffer(int fd,void *buf,int count)
{
    void *pts=buf;
    int status=0,n;

    if(count<0)
        return -1;

    while(status!=count)
    {
        if((n=read(fd,pts+status,count-status))==-1)
        {
            printf("error,failed to write!\n");
            exit(254);
        }else if(n<0){

            return n;
        }
        status+=n;
    }

    return status;
}
int readnlstring(int socket,char *buf,int maxlen,char delim)
{
    int status;
    int count=0;
    while(count<maxlen-1)
    {
        if((status=read_buffer(socket,buf+count,1))<1)
        {
            return -1;
        }

        if(buf[count]==delim)
        {
            buf[count]=0x00;
            return count;
        }
        count++;
    }

    buf[count]=0x00;

    return count;
}
