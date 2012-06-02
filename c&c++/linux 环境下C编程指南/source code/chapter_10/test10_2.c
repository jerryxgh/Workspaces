#include <stdio.h>
#include <signal.h>
#include <stdarg.h>
void intermithandler(int signumber);
void continuehandler(int signumber);
void terminatehandler(int signumber);
int main(void)
{
    char buffer1[100],buffer2[100];
    int i;
    if(signal(SIGINT,&terminatehandler)==SIG_ERR)
    {
        printf("Couldn¡¯t register signal handler for SIGINT!\n");
    }
    if(signal(SIGTSTP,&terminatehandler)==SIG_ERR)
    {
        printf("Couldn¡¯t register signal handler for SIGTSTP!\n");
    }
    if(signal(SIGCONT,&continuehandler)==SIG_ERR)
    {
        printf("Couldn¡¯tregister signal handler for SIGCONT!\n");
    }
    if(signal(SIGTERM,&terminatehandler)==SIG_ERR)
    {
        printf("Couldn¡¯t register signal handler for SIGTERM!\n");
    }
    printf("Pid of This Process:%6d.",getpid());
    for( ; ; )
    {
        fgets(buffer1,sizeof(buffer1),stdin);
        for(i=0;i<100;i++)
        {
            if(buffer1[i]>=97 && buffer1[i]<=122)
                buffer2[i]=buffer1[i]-32;
            else
                buffer2[i]=buffer1[i];
        }
        printf("Your input is: %s\n\n",buffer2);
    }
    return 0;
}
void intermithandler(int signumber)
{
    printf("catch signal %d.;\n",signumber);
}
void continuehandler(int sinnumber)
{
    printf("Continue code.\n");
}
void terminatehandler(int signumber)
{
    printf("signal SIGTERM.\n");
    exit(0);
}

