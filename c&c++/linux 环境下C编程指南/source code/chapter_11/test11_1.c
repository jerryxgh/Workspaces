#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

size_t readn(int fd,void *vptr,size_t n)
{
    size_t nleft;
    size_t nread;
    char *ptr=vptr;
    nleft=n;
    while(nleft>0)
    {
        if((nread£½read(fd,ptr,nleft))<0)
        {
            if(errno==EINTR)
                nread=0;
            else
                return -1;
        }else if(nread==0)
            break;
        nleft=nleft-nread;
        ptr+=nread;
    }
    return(n-nleft);
}


size_t written(int fd,const char *vptr,size_t n)
{
    size_t nleft;
    size_t nwrite;
    const char *ptr=vptr;
    nleft=n;
    while(nleft>0)
    {
        if((nwrite=write(fd,ptr,nleft))<0)
        {
            if(errno==EINTR)
                nwrite=0;
            else
                ruturn(-1);
        }
        nleft=nleft-nwrite;
        ptr+=nwrite;
    }
    return(n-nleft);
}

size_t readline(int fd,void *vptr,size_t maxlen)
{
 again:
    size_t n,rc;
    char c,*ptr=vptr;
    for(n=1;n<maxlen;n++)
    {
        if((rc=read(fd,&c,1))==1)
        {
            *ptr++=c;
            if(c=='\n')
                break;
        }else if(rc==0){
            if(n==1)
                return 0;
            else
                break;
        }else{
            if(errno==EINTR)
                goto again;
            return(-1);
        }
    }
    *ptr=0x00;
    return(n);
}
