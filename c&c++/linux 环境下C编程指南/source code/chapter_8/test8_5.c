#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    if(open("tempfile",O_RDWR)<0)
    {
        printf("open error!\n");
        exit(1);
    }

    if(unlink("tempfile")<0)
    {
        printf("unlink error");
        exit(1);
    }

    printf("file unlinked.\n");
    sleep(15);
    printf("done\n");
    exit(0);
}
