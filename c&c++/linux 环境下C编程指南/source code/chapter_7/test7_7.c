#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define LINESIZE 1024
int main(void)
{
    int n,fd[2];
    pid_t pid;
    char line[LINESIZE];
    if(pipe(fd)<0)
    {
        printf("pipe error");
        exit(1);
    }
    if((pid=fork())<0)
    {
        printf("fork error");
        exit(1);
    }else if(pid==0)
    {
        close(fd[0]);
        write(fd[1],"Hello!\n",6);
        close(fd[1]);
    }else{
        close(fd[1]);
        if(0 > waitpid(pid,NULL,0))
        {
            printf("waitpid error!\n");
            exit(0);
        }
    }
}
