#include<stdio.h>
#include<signal.h>
#include <stdarg.h> 
void intermithandler(int signumber);
void continuehandler(int signumber);
void terminatehandler(int signumber);
int main(void)
{
    char buffer[100];
    if(signal(SIGINT,&terminatehandler)==SIG_ERR)
    {
        printf("Could not register signal handler!\n");
    }
    if(signal(SIGTSTP,&terminatehandler)==SIG_ERR)
    {
        printf("Could not register signal handler!\n");
    }
    if(signal(SIGCONT,&continuehandler)==SIG_ERR)
    {
        printf("Couldn¡¯t register signal handler!\n");
    }
    if(signal(SIGTERM,&terminatehandler)==SIG_ERR)
    {
        printf("Could not register signal handler!\n");
    }
    printf("Pid of This Process:%6d.\n",getpid());
    for( ; ; )
    {
        fgets(buffer,sizeof(buffer),stdin);
        if(strcmp(buffer,"intermit\n")==0)
        {
            raise(SIGINT);
        }
        else if(strcmp(buffer,"stop\n")==0)
        {
            raise(SIGTSTP);
        }
        else if(strcmp(buffer,"continue\n")==0)
        {
            raise(SIGCONT);
        }
        else if(strcmp(buffer,"quit\n")==0)
        {
            raise(SIGTERM);
        }
        else
        {
            printf("Your input is: %s\n\n",buffer);
        }
    }
    return 0;
}
void intermithandler(int signumber)
{
    printf("signal %d.;\n",signumber);
}
void continuehandler(int sinnumber)
{
    printf("Continue code.\n");
}
void terminatehandler(int signumber)
{
    printf("signal SIGTERM.\n");
    return;
}

