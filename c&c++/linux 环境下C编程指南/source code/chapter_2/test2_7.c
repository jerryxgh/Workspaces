#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int write_buffer(int fd,const void *buf,int count);
int main(void)
{
    int outfile;
    char *mapped;

    char *ptr;

    if((outfile=open("test.dat",O_RDWR|O_CREAT|O_TRUNC,0640))==-1)
    {
        printf("COULDN'T OPEN THIS FILE!\n");
        exit(254);
    }

    lseek(outfile,1000,SEEK_SET);

    if(write(outfile,"\0",1)==-1)
    {
        printf("ERROR,WRITE FAILED!\n");
        exit(254);
    }

    mapped=mmap(NULL,1000,PROT_READ|PROT_WRITE,MAP_SHARED,outfile,0);

    if(NULL==mapped)
    {
        printf("ERROR,MMAP FAILED!\n");
        exit(254);
    }

    ptr=mapped;

    printf("PLEASE ENTER A NUMBER:");
    fgets(mapped,80,stdin);
    ptr+=strlen(mapped);
    ptr++;
    sprintf(ptr,"YOUR NUMBER TIMES TWO IS:%d.\n",atoi(mapped)*2);
    printf("YOUR NUMBER TIMES TWO IS:%d\n",atoi(mapped)*2);
    msync(mapped,1000,MS_SYNC);
    munmap(mapped,1000);
    if(close(outfile))
    {
        printf("POSSIBLY SERIOUS ERROR,CLOSE FILE FAILED");
        exit(254);
    }
    return 0;
}

int write_buffer(int fd,const void *buf,int count)
{
    const void *pts=buf;
    int status=0,n;
    if(count<0)
        return(-1);
    while(status!=count)
    {
        if((n=write(fd,"\0",1))==-1)
        {
            printf("ERROR,WRITE FAILED!\n");
            exit(254);
        }
        if(n<0)
            return(n);
        status+=n;
    }
    return(status);
}

