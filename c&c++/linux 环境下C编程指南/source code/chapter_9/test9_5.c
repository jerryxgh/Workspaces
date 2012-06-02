#include <stdio.h>
#include <stdlib.h>

int character;
char token[1024];
char* ptr;
void lexicure(void);
int main(void)
{ 
    ptr = token;
    lexicure();
    while(character)
    {
        fprintf(stdout,"%s",token);
        lexicure();
    }
    exit(0);
}

void lexicure(void)
{
    character=getchar();

    if(('A'<=character)&&('Z'>=character))    
    { 
        while((('A'<=character)&&('Z'>=character))||(('0'<=character)&&('9'>=character)))
        {
            *ptr++=character;
            character=getchar();
        }

        if(character!=EOF)
            ungetc(character,stdin);
        else
            ungetc(0,stdin);



    }else if(('0'<=character)&&('9'>=character))
    {
        while(('0'<=character)&&('9'>=character))
        {
            *ptr++=character;
            character=getchar();
        }

        if(character!=EOF)
            ungetc(character,stdin);
        else
            ungetc(0,stdin);

    }

}

