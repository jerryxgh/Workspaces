#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
int main(void) 
{
    float value,total[10];
    int count,label;
    FILE *fp;
    for(count=0;count<10;count++)
        total[count]=0;
    if(NULL==(fp=fopen("test.dat","r")))
    {
        printf("Error in open file.\n");
        exit(1);
    }
    while(EOF!=fscanf(fp,"%d %g",&label,&value))
    {
        total[label]+=value;
    }
    for(count=0;count<10;count++)
    {
        printf("%d: %f\n",count,total[count]);
    }
    return 0;
}

