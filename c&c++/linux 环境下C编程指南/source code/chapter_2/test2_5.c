#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void upcase(char *inputstring,char** newstring);
int main(void)
{
    char *string = NULL;
    upcase("Hello",&string);
    printf("str1=%s\n",string);
    upcase("Goodbye",&string);
    printf("str2=%s\n",string);
    free(string);
    return 0;
}

void upcase(char *inputstring,char** newstring)
{
    int counter;
    if(NULL==*newstring)
    {
        if(NULL ==(*newstring=malloc(strlen(inputstring)+1)))
        {
            printf("ERROR ALLOCATING MEMORY!\n");
            exit(255);
        }

    }else{

        if(NULL==(*newstring=realloc(*newstring,sizeof(inputstring)+1)))
        {
            printf("ERROR REALLOCATING MEMORY!\n");
            exit(255);
        }
    }

    strcpy(*newstring,inputstring);

    for(counter=0;counter<strlen(*newstring);counter++)
    {
        if(*newstring[counter]>=97 && *newstring[counter]<=122)
            *newstring[counter]-=32;
    }

}

