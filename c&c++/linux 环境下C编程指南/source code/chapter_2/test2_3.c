#include <stdio.h>
#include <string.h>
void upcase(char *inputstring,char *newstring);
int main(void)
{
    char str1[4],str2[4];
    upcase("Hello",str1);
    printf("str1=%s\n",str1);
    return 0;
}
void upcase(char *inputstring,char *newstring)
{
    int counter;
    strncpy(newstring,inputstring,sizeof(newstring)-1);
    for(counter=0;counter<strlen(newstring)-1;counter++)
    {
        if(newstring[counter]>=97 && newstring[counter]<=122)
            newstring[counter]-=32;
    }
    *(newstring+strlen(newstring)) = 0x00;
    return newstring;
}

