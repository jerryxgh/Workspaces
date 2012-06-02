#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#define ARRAY_SIZE 4000
#define MALLOC_SIZE 10000
#define SHM_SIZE 10000
#define SHM_MODE (SHM_R|SHM_W)

void printf_shmpemid(struct shmid_ds *buf);
char array[ARRAY_SIZE];

int main(void)
{
    int shmid;
    char *ptr,*shmptr;
    struct shmid_ds shmds;
    printf("array[] from %x to %x\n",&array[0],&array[ARRAY_SIZE]);
    printf("stack around %x \n",&shmid);
    if(NULL==(ptr=malloc(MALLOC_SIZE)))
    {
        fprintf(stderr,"malloc error\n");
        exit(1);
    }
    if((shmid=shmget(IPC_PRIVATE,SHM_SIZE,SHM_MODE))<0)
    {
        fprintf(stderr,"shmget error!\n");
        exit(1);
    }
    if ((shmptr=shmat(shmid,0,0))==(void*)-1)
    {
        fprintf(stderr,"shmat error!\n");
        exit(1);
    }
    if(shmctl(shmid,IPC_STAT,&shmds)==-1)
    {
        fprintf(stderr,"GET shmid_ds ERROR!\n");
        exit(1);
    }
    printf_shmpemid(&shmds);
    if(shmctl(shmid,IPC_RMID,0)<0)
    {
        fprintf(stderr,"shmctl error\n");
        exit(1);
    }
    exit(0);
}

void printf_shmpemid(struct shmid_ds *buf)
{
    printf("Struct ipc_perm:\n");
    printf(" uid=%d:\n",buf->shm_perm.uid);
    printf(" gid=%d:\n",buf->shm_perm.gid);
    printf(" cuid=%d:\n",buf->shm_perm.cuid);
    printf(" cgid=%d:\n",buf->shm_perm.cgid);
    return;
}

