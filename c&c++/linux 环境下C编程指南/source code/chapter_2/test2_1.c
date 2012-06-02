#include <stdio.h> 
#include <string.h>
char *upcase(char *inputstring);
int main(void)
{
    char *str1;
    str1=upcase("Hello");
    printf("str1=%s\n",str1);
    return 0;
}
char *upcase(char *oldstring)
{
    int counter;
    char newstring[100];
    strcpy(newstring,oldstring);
    for(counter=0;counter<strlen(newstring);counter++)
    {
        if(newstring[counter]>=97 && newstring[counter]<=122)
            newstring[counter]-=32;
    }
    return newstring;
}

