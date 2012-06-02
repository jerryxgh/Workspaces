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

void str_ser1(int sockfd);

int main(void)
{
    int sockfd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int addr_len;
    if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1)
    {
        printf("error in socket");
        exit(1);
    }

    my_addr.sin_family=AF_INET;
    my_addr.sin_port=htons(MYPORT);
    my_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    memset(my_addr.sin_zero,0,8);
    addr_len = sizeof(struct sockaddr);

    if(bind(sockfd,(const struct sockaddr *)&my_addr,addr_len)==-1)
    {
        printf("error in binding");
        exit(1);
    }
    str_ser1(sockfd);
    close(sockfd);
    exit(1);
}
void str_ser1(int sockfd)
{
    char recvs[MAXSIZE];
    int n=0,len;
    struct sockaddr addr;
    while(1)
    {
        len=sizeof(struct sockaddr_in);
        printf("waiting for data....\n");
        if((n=recvfrom(sockfd,recvs,MAXSIZE,0,(struct sockaddr*)&addr,&len))==-1)
        {
            printf("error receiving");
            exit(1);
        }
        
        *(recvs+n-1)= 0x00;
        printf("Received [%s],return to client immediately\n",recvs);

        sendto(sockfd,recvs,n,0,(struct sockaddr*)&addr,len);
    }
}

