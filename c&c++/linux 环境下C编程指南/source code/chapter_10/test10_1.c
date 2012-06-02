#include <stdio.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>

void terminatehandler(int signumber);

int main(void) 
{
    char buffer1[100],buffer2[100];
    int i;
    if(signal(SIGTERM,&terminatehandler)==SIG_ERR)
    {
        printf("Couldn't register signal handler!\n");
    }
    printf("Pid of This Process:%6d.",getpid());
    for( ; ; )
    {
        fgets(buffer1,sizeof(buffer1),stdin);
        for(i=0; i<100; i++)
        {
            if(buffer1[i]>=97&&buffer1[i]<=122)
                buffer2[i]=buffer1[i]-32;
            else
                buffer2[ i ]=buffer1[ i ];
        }
        printf("Your input is: %s\n\n",buffer2);
    }
    return 0;
}
void terminatehandler(int signumber)
{
    printf("catch signal SIGTERM.\n");
    exit(0);
}

