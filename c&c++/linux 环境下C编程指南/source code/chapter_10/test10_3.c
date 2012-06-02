#include<stdio.h>
#include<signal.h>
#include<stdarg.h>
#include<string.h>
void terminatehandler(int signumber);
int main(void)
{
    char buffer1[100],buffer2[100]; 
    int i;
    struct sigaction act;
    act.sa_handler=terminatehandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    if(sigaction(SIGTERM,&act,NULL)==-1)
    {
        printf("Could not register signal handler!\n");
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
void terminatehandler(int signumber)
{
    printf("signal SIGTERM.\n");
    return;
}

