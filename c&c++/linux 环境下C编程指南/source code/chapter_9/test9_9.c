#include <stdio.h>
int main(void)
{
    char line[1024];
    char *name;
    FILE *fp;
    if(NULL==(fp=fopen(name=tempnam("/home/stevents",NULL),"w+")))
    {
        printf("tempfile error");
        exit(1);
    }
    unlink(name);
    if(!fputs("This is a temp file.\n",fp))
    {
        printf("write error.\n");
        exit(1);
    }
    rewind(fp);
    if(!fgets(line,sizeof(line),fp))
    {
        printf("read error.\n");
        exit(1);
    }
    printf("%s\n",line);
    exit(0);
}

