#include <stdio.h>
#include <signal.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#if defined(__Linux__)&&!defined(SI_KERNEL)

#define SI_KERNEL 0x80
#endif

int my_printf(const char *fmt,...);
void sighandler(int signumber,siginfo_t *info,void *extre);
void continuehandler(int signumber,siginfo_t *info,void *extre);

char buffer[200];

int main(void)
{
    struct sigaction act;
    sigset_t blockset,pending;
    int pendingcount;
    strcpy(buffer,"None\n");
    sigemptyset(&blockset);
    act.sa_mask=blockset;
    act.sa_flags=SA_SIGINFO;
    act.sa_sigaction=&sighandler;
    if(sigaction(SIGTERM,&act,NULL)==-1)
    {
        my_printf("Could not register signal for SIGINT.\n");
    }
    act.sa_sigaction=&continuehandler;
    if(sigaction(SIGCONT,&act,NULL)==-1)
    {
        my_printf("Could not register signal for SIGCONT.\n");
    }
    sigaddset(&blockset,SIGTERM);
    sigaddset(&blockset,SIGINT);
    for(;;)
    {
        sigprocmask(SIG_BLOCK,&blockset,NULL);
        fgets(buffer,sizeof(buffer),stdin);
        printf("Input:%s",buffer);
        sigpending(&pending);
        pendingcount=0;
        if(sigismember(&pending,SIGINT))
            pendingcount++;
        if(sigismember(&pending,SIGTERM))
            pendingcount++;
        if(pendingcount)
        {
            my_printf("There are %d signals pending.\n",pendingcount);
        }
        sigprocmask(SIG_UNBLOCK,&blockset,NULL);
    }
    return 0;
}
int my_printf(const char *fmt,...)
{
    va_list args;
    struct tm a;
    struct tm *tstruct =&a;
    time_t tsec;
    tsec=time(NULL);
    tstruct=localtime(&tsec);
    printf("%02d:%02d:%02d %05d\n",tstruct->tm_hour,tstruct->tm_min,tstruct->tm_sec,getpid());
    va_start(args,fmt);
    return vprintf(fmt,args);
}
void sighandler(int signumber,siginfo_t *info,void *extre)
{
    my_printf("Caught signal %d from",signumber);
    switch(info->si_code)
    {
        case SI_USER: printf("a user process \n");
                      break;
        case SI_KERNEL: printf("the kernel \n");
                        break;
        default: printf("something,strange \n");
    }
}
void continuehandler(int signumber,siginfo_t *info,void *extre)
{
    my_printf("Continuing.\n");
    my_printf("Your last input was: %s",buffer);
}

