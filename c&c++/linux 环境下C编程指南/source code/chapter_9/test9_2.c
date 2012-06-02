#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE 1024
int main(void)
{
    char buf[BUF_SIZE];
    FILE *source,*backup;
    if(NULL==(source=fopen("source.dat","r")))
    {
        printf("Error in opening file.\n");
        exit(1);
    }
    if(NULL==(backup=fopen("backup.dat","w")))
    {
        printf("Error in creating file.\n");
        exit(1);
    }
    while(fread(buf,sizeof(buf),1,source)==1)
    {
        if(fwrite(buf,sizeof(buf),1,backup))
        {
            printf("Error in wrinting file.\n");
            exit(1);
        }

        if(ferror(source)==0)
        {
            printf("Error in reading file.\n");
            exit(1);
        }
        if(!fclose(source))
        {
            printf("Error in close file.\n");
            exit(1);
        }
        if(fclose(backup))
        {
            printf("Error in close file.\n");
            exit(1);
        }
    }
}

