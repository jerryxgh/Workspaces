#include <sys/types.h>
#include <linux/ipc.h>
#include <linux/sem.h>
#include <stdio.h>
#include <stdlib.h>

#define SEM_NUM 10
#define SEM_MODE (IPC_CREAT|0660)

void printf_stat(union semun *arg);

int main(void)
{
    int semid;
    union semun semopts;
    struct semid_ds semds;
    if((semid=semget(IPC_PRIVATE,SEM_NUM,SEM_MODE))==-1)
    {
        fprintf(stderr,"semget error!\n");
        exit(1);
    }
    semopts.buf=&semds;
    if(semctl(semid,IPC_STAT,semopts)==-1)
    {
        fprintf(stderr,"get semid_ds error!\n");
        exit(1);
    }
    printfmode(&semopts);
    changemode(semid,0600);
    if(semctl(semid,IPC_STAT,semopts)==-1)
    {
        fprintf(stderr,"get semid_ds error!\n");
        exit(1);
    }
    printfmode(&semopts);
    if(semctl(semid,IPC_RMID,0)<0)
    {
        fprintf(stderr,"semctl error\n");
        exit(1);
    }
    exit(0);
}
void printfmode (union semun *arg)
{
    printf(" mode=%d:\n",arg->buf->sem_perm.mode);
    return;
}
void changemode(int sid,char *mode)
{
    int rc;
    union semun semopts;
    struct semid_ds mysemds;
    semopts.buf=&mysemds;
    rc=semctl(sid,0,IPC_STAT,semopts);
    if(rc==-1)
    {
        printf("semctl error!\n");
        exit(1);
    }
    sscanf(mode,"%ho",&semopts.buf->sem_perm.mode);
    semctl(sid,0,IPC_SET,semopts);
    return;
}

