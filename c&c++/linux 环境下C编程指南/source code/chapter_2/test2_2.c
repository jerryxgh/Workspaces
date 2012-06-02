#include <stdio.h>
#include <string.h>
void upcase(char *inputstring,char *newstring);
int main(void)
{
    char str1[100],str2[100];
    upcase("Hello",str1);
    upcase("Goodbye",str2);
    printf("str1=%s,str2=%s\n",str1,str2);
    return 0;
}
void upcase(char *inputstring,char *newstring)
{
    int counter;
    strcpy(newstring,inputstring);
    for(counter=0;counter<strlen(newstring);counter++)
    {
        if(newstring[counter]>=97 && newstring[counter]<=122)
            newstring[counter]-=32;
    }
}

