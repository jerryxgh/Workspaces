#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[])
{
    char symname[]={"mysym"};
    char buf[80];
    if(argc<2)
        return 0;
    if(symlink(argv[1],symname)==-1)
    {
        printf("symlink error!\n");
        exit(1);
    }
    if(!readlink(symname,buf,sizeof(buf)))
    {
        printf("readlink error!\n");
        exit(1);
    }
    printf("name of symbol link: %s.\n",buf);
    exit(0);
}
