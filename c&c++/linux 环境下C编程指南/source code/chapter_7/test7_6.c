#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 1024

int main(void)
{
    int n,fd[2];
    pid_t pid;
    char line[MAXLINE];
    if(pipe(fd)<0)
    {
        fprintf(stderr,"pipe error\n");
        exit(1);
    }
    if((pid=fork())<0)
    {
        fprintf(stderr,"fork error\n");
        exit(1);
    }else if(pid==0){
        close(fd[1]);
    }
    else{
        close(fd[0]);
    }
    exit(0);
}

