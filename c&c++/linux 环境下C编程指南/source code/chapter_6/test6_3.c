#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
    pid_t pid;
    if((pid=vfork())<0)
    {
        printf("fork error!\n");
        exit(1);
    }
    else if(pid==0)
    {
        printf("Child process PID: %d.\n",getpid());
        setenv("PS1", "CHILD\\$", 1);
        printf("Process%4d: calling exec.\n",getpid());
        if(execl("/bin/sh", "/bin/sh", "arg2", NULL)<0)
        {
            printf("Process%4d: execle error!\n",getpid());
            exit(0);
        }
        printf("Process%4d: You should never see this because the child is already gone. \n",getpid());
        printf("Precess%4d: The child process is exiting.\n");
    }
    else
    {
        printf("Parent process PID:%4d.\n", getpid());
        printf("Process%4d: The parent has fork process %d.\n", pid);
        printf("Process%4d: The child has called exec or has exited.\n", 
                getpid());
    }
    return 0;
}

