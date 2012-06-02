#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 1024
#define MYKEY 24

int main(void)
{
    int shmid;
    char *shmptr;

    if(shmid=shmget(MYKEY,BUF_SIZE,IPC_CREAT)==-1)
    {
        printf("shmget error!\n");
        exit(1);
    }
    if((shmptr=shmat(shmid,0,0))==(void*)-1)
    {
        fprintf(stderr,"shmat error!\n");
        exit(1);
    }
    while(1)
    {
        printf("input a string:");
        scanf("%s",shmptr);
    }
    exit(0);
}

