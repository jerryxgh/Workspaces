#include <stdio.h>
#include <signal.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
int my_printf(const char *fmt,...);
void sighandler(int signum);
void continuehandler(int signumber);
char buffer[200];
int main(void)
{
    sigset_t blockset,pending;
    int pendingcount;
    strcpy(buffer,"None\n");
    if(signal(SIGINT,&sighandler)==SIG_ERR)
    {
        my_printf("Couldn't register signal handler for SIGINT.\n");
    }
    if(signal(SIGTERM,&sighandler)==SIG_ERR)
    {
        my_printf("Couldn't register signal handler for SIGTERM.\n");
    }
    if(signal(SIGCONT,&sighandler)==SIG_ERR)
    {
        my_printf("Couldn't register signal handler for SIGCONT.\n");
    }
    sigemptyset(&blockset);
    sigaddset(&blockset,SIGINT);
    sigaddset(&blockset,SIGTERM);

    for(;;)
    {
        sigprocmask(SIG_BLOCK,&blockset,NULL);
        fgets(buffer,sizeof(buffer),stdin);
        my_printf("Input: %s",buffer);
        sigpending(&pending);
        pendingcount=0;
        if(sigismember(&pending,SIGINT))
            pendingcount++;
        if(sigismember(&pending,SIGTERM))
            pendingcount++;
        if(pendingcount)
        {
            my_printf("There are %d signals. \n",pendingcount);
        }
        sigprocmask(SIG_UNBLOCK,&blockset,NULL);
    }
    return 0;
}
int my_printf(const char *fmt,...)
{
    va_list args;
    struct tm *tstruct;
    time_t tsec;
    tsec=time(NULL);
    tstruct=localtime(&tsec);
    printf("%02d:%02d:%02d %05d\n",tstruct->tm_hour,tstruct->tm_min, tstruct->tm_sec,getpid());
    va_start(args,fmt);
    return vprintf(fmt,args);
}
void sighandler(int signumber)
{
    my_printf("Caught sight %d.\n",signumber);
}
void continuehandler(int signumber)
{
    my_printf("Continuint.\n");
    my_printf("Your last input was: %s",buffer);
}

