#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

int newshm(int size)
{
    int shmid;
    if(shmid=shmget(IPC_PRIVATE,size,0)==-1)
    {
        printf("ERROR,GET SHARE MEMEROY FAILED!\n");
        exit(1);
    }
    return shmid;
}

//test function -- newshm

/*
int main(void)
{
    newshm(1000);

    return 0;
}
*/
