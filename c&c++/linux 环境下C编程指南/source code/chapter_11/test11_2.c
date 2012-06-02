#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <stdarg.h>     /* ISO C variable aruments */
#include <string.h>

static void err_doit(int, int, const char *, va_list);

void err_sys(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
    exit(1);
}
static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
    char    buf[1024];
    vsnprintf(buf, 1024, fmt, ap);
    if (errnoflag)
        snprintf(buf+strlen(buf), 1024-strlen(buf), ": %s",
                strerror(error));
    strcat(buf, "\n");
    fflush(stdout);     /* in case stdout and stderr are the same */
    fputs(buf, stderr);
    fflush(NULL);       /* flushes all stdio output streams */
}

int main(int argc,char **argv)
{
    char *ptr,**pptr;
    char str[INET6_ADDRSTRLEN];
    struct hostent *hptr;
    while(--argc>0)
    {
        ptr=*(++argv);
        if((hptr=gethostbyname(ptr))==NULL)
        {
            printf("gethostbyname call error:%s,%s\n",ptr,hstrerror(h_errno));
            continue;
        }
        printf("canonical name:%s\n",hptr->h_name);
        for(pptr=hptr->h_aliases;*pptr!=NULL;pptr++)
            printf("the aliases name is:%s\n",*pptr);
        switch(hptr->h_addrtype)
        {
            case AF_INET:
            case AF_INET6:
                pptr=hptr->h_addr_list;
                for(;*pptr!=NULL;pptr++)
                    printf("address:%s\n",inet_ntop(hptr->h_addrtype,*pptr,str,sizeof(str)));
                break;
            default:
                err_sys("unknown addrtype");
                break;
        }
    }
    exit(0);
}

