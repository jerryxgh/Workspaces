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
    while(fgets(buf,sizeof(buf),source))
    {
        if(!fputs(buf,backup))
        {
            printf("Error in writing file.\n");
            exit(1);
        }
    }

    if(ferror(source))
    {
        printf("Error in reading file.\n");
        exit(1);
    }

    if(fclose(source))
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

