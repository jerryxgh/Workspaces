#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <stdarg.h> 
#include <string.h>

#define MAXSIZE 80
#define MYPORT 3490
#define BACKLOG 10
#define BUFSIZE 100

void str_ser(int sockfd);

int main()
{
    int sockfd,new_fd,numbytes,ret;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sin_size;
    char *buf;
    pid_t pid;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        printf("error in socket");
        exit(1);
    }

    my_addr.sin_family=AF_INET;
    my_addr.sin_port=htons(MYPORT);
    my_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    bzero(&(my_addr.sin_zero),8);
    ret=bind(sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr));
    if(ret<0)
    {
        printf("error in binding");
        exit(1);
    }
    ret=listen (sockfd,BACKLOG);
    if(ret<0)
    {
        printf("error in listening");
        exit(1);
    }

    while(1)
    {

        sin_size=sizeof(struct sockaddr_in);
        new_fd=accept(sockfd,(struct sockaddr *)&their_addr,&sin_size);
        if(new_fd<0)
        {
            printf("error in accept");
            exit(1);
        }

        if((pid=fork())==0)
        {
            close(sockfd);
            str_ser(new_fd);

            close(new_fd);
            exit(0);
        }else
            close(new_fd);

    }
    close(sockfd);
    exit(0);
}

void str_ser(int sockfd)
{
    char recvs[MAXSIZE];
    int n=0;
    while(1)
    {
        if((n=recv(sockfd,recvs,MAXSIZE,0))==0)
            return;

        send(sockfd,recvs,n,0);
    }
}
