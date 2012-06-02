#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char line[1<<10];

    FILE *fp;
    tmpnam(NULL);
    if(NULL==(fp=tmpfile()))
    {
        printf("tempfile error");
        exit(1);
    }
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

