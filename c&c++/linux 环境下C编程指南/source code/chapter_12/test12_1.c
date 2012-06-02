#include <stdlib.h>
#include <stdio.h>
#include <term.h>
#include <curses.h>
#define NUMCAPS 3

int main(void)
{ 
    int j;
    int retval=0;
    char *buf;
    char *boolcaps[NUMCAPS]={"am","bce","km"};
    char *numcaps[NUMCAPS]={"cols","lines","colors"};
    char *strcaps[NUMCAPS]={"cup","flash","hpa"};

    if(setupterm(NULL,fileno(stdin),NULL)!=OK)
    {
        perror("setupterm()");
        exit(EXIT_FAILURE);
    }
    for(j=0;j<NUMCAPS;++j)
    {
        retval=tigetflag(boolcaps[j]);
        if(retval==FALSE)
            printf(" '%s' unsupported\n",boolcaps[j]);
        else
            printf(" '%s' supported\n",boolcaps[j]);
    }
    
    fputc('\n',stdout);
    for(j=0;j<NUMCAPS;++j)
    {
        retval=tigetnum(numcaps[j]);
        if(retval==ERR)
            printf(" '%s' unsupported\n",numcaps[j]);
        else
            printf(" '%s' is %d\n",numcaps[j],retval);
    }
    fputc('\n',stdout);

    for(j=0;j<NUMCAPS;++j)
    {
        buf=tigetstr(strcaps[j]);
        if(buf==NULL)
            printf(" '%s' unsupported\n",strcaps[j]);
        else
            printf(" '%s' is %s\n",strcaps[j],buf);
    }
}
