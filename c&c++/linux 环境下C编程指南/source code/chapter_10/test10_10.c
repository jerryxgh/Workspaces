#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

int my_printf(const char *fmt,...);
void sighandler(int signum);
void continuehandler(int signumber);
char buffer[200];

int main(void)
{
    struct itimerval itimer;
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
    itimer.it_interval.tv_usec=0;
    itimer.it_interval.tv_sec=0;
    itimer.it_value.tv_usec=0;
    itimer.it_value.tv_sec=0;
    for(;;)
    {
        sigprocmask(SIG_BLOCK,&blockset,NULL);
        alarm(15);
        fgets(buffer,sizeof(buffer),stdin);
        my_printf("input:%s",buffer);
        sigpending(&pending);
        pendingcount=0;
        if(sigismember(&pending,SIGINT))
            pendingcount++;
        if(sigismember(&pending,SIGTERM))
            pendingcount++;
        if(pendingcount)
        {
            my_printf("There are %d signals.\n",pendingcount);
        }
        sigprocmask(SIG_UNBLOCK,&blockset,NULL);
        if(strcmp(buffer,"exit\n")==0)
        {
            raise(SIGKILL);
        }
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
    printf("%02d:%02d:%02d %05d\n",tstruct->tm_hour,tstruct->tm_min,tstruct->tm_sec,getpid());
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

void alarmhandler(int signumber)
{
    my_printf("No activity for 15 seconds,exiting.\n");
    exit(0);
}

