#include <sys/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int vfsf(FILE* fp,char *fmt,...)
{
    va_list argptr;
    int cnt;
    va_start(argptr,fmt);
    cnt = vfscanf(fp,fmt,argptr);
    va_end(argptr);
    return cnt;
}

int vpf(char *fmt,...)
{
    va_list argptr;
    int cnt;

    va_start(argptr,fmt);
    cnt = vprintf(fmt,argptr);
    va_end(argptr);
    return cnt;
}

int main(void)
{
    float value,total[10];
    int count,label;
    va_list ap_read,ap_write;
    FILE *fp;
    for(count=0;count<10;count++)
        total[count]=0;
    if(NULL==(fp=fopen("test.dat","r")))
    {
        printf("Error in open file.\n");
        exit(1);
    }

    while(EOF!=vfsf(fp,"%d %g",&label,&value))
    {
        total[label]+=value;
    }


    for(count=0; count<10; count++)
    {
        vpf("%d: %f\n",count,total[count]);
    }
    return 0;
}

