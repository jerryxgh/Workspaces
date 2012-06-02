#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h> 

int main()
{
    char key;
    FILE *stream;/*流的指针*/
    
    printf("Please input a letter.\n");
    key=getchar();/*读入一个字符*/
    
    if((stream=fopen("test","w"))==(FILE*)0)/*将流与一个文件联系起来,并判断是否成功*/
    {
        fprintf(stderr,"Error opening file.\n");
        exit(1);
    }
    /*将缓冲区设定为行缓冲*/

    
    setlinebuf(stream);

    /*将对应的缓冲区清空*/
    
    __fpurge(stream);
    
    
    /*对流的读写操作*/
    fprintf(stream,"The letter that you input is %c.\n",key);
    /*在程序结束前关闭流,并判断操作是否成功*/
    if((fclose(stream))==EOF)
    {
        fprintf(stderr,"Error closing file!\n");
        exit(1);
    }
    
    exit(0);
}
