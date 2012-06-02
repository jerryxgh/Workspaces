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

#define MYPORT 4950
#define MAXSIZE 80

void str_cli1(FILE *fp,int sockfd,struct sockaddr *addr,int addrlen);

int main(int argc,char *argv[])
{
    int sockfd,ret,len;
    struct sockaddr_in ser_addr,their_addr;
    char *myname;
    struct in_addr **addrs;
    struct hostent *sh;
    if(argc!=2)
    {
        printf("parameters not match.");
        exit(0);
    }

    if((sh=gethostbyname(argv[1]))==NULL)
    {
        printf("error when gethostbyname");
        exit(0);
    }

    sockfd=socket(AF_INET,SOCK_DGRAM,0 );
    if(sockfd<0)
    {
        printf("error in socket");
        exit(1);
    }

    their_addr.sin_family=AF_INET;
    their_addr.sin_port=htons(MYPORT);
    their_addr.sin_addr=*((struct in_addr *)sh->h_addr);
    memset(their_addr.sin_zero,0,8);
    str_cli1(stdin,sockfd,(struct sockaddr *)&their_addr,sizeof(struct sockaddr));

    close(sockfd);
    exit(0);
}
void str_cli1(FILE *fp,int sockfd,struct sockaddr *addr,int addrlen)
{
    char sends[MAXSIZE],recvs[MAXSIZE];
    int n=0;
    while(fgets(sends,MAXSIZE,fp)!=NULL)
    {
        if(0==sendto(sockfd,sends,strlen(sends),0,addr,addrlen))
        {
            printf("error sending date\n");
        }

        if((n=recvfrom(sockfd,recvs,MAXSIZE,0,NULL,NULL))==0)
        {
            printf("error receiving data\n");
            exit(1);
        }
        recvs[n-1]=0x00;
        printf("echo data [%s]",recvs);
    }
}

