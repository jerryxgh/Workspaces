#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TAG_datastruct{ 
    char* string;
    int checksum;
}datastruct;

datastruct* getinput(void);
void printmessage(datastruct* todisp);

int main(void)
{
    int counter;
    int maxval=0;
    datastruct* svalues[200];

    for(counter=0;counter<200;counter++)
    {
        svalues[counter]=getinput();
        if(!svalues[counter]) break;
        maxval=counter;
    }

    printmessage(svalues[maxval/2]);

    return 0;
}

datastruct* getinput(void)
{
    char input[80];
    datastruct* instruct;
    int counter;

    printf("Ente a string,or leave blank when done:");
    fgets(input,79,stdin);
    input[strlen(input)-1]=0;//remove the "\n"
    if(strlen(input)==0)
        return NULL;
    instruct=malloc(sizeof(datastruct));
    instruct->string=strdup(input);
    instruct->checksum=0;
    for(counter=0;counter<strlen(instruct->string);counter++)
    {
        instruct->checksum+=instruct->string[counter];
    }
    return instruct;
}

void printmessage(datastruct* todisp)
{
    printf("This structure has a checksum of %d.Its string is:\n",
            todisp->checksum);
    puts(todisp->string);
}

