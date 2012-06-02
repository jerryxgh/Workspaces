#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char *upcase(char *inputstring);
int main(void)
{
    char *str1;
    str1=upcase("Hello");
    printf("str1=%s \n",str1);
    free(str1);
    return 0;
}
char *upcase(char *inputstring)
{
    char *newstring;
    int counter;
    if(NULL==(newstring=malloc(strlen(inputstring)+1)))
    {
        printf("ERROR ALLOCATING MEMORY!\n");
        exit(255);
    }
    strcpy(newstring,inputstring);
    for(counter=0;counter<strlen(newstring);counter++)
    {
        if(newstring[counter]>=97 && newstring[counter]<=122)
            newstring[counter]-=32;
    }
    return newstring;
}

