#include <stdio.h>
#include <stdlib.h>

int main()
{
    int j,a[10];
    for(j=0;j<=9;j++) 
        a[j]=j;
    for(j=9;j>=0;j--)
        printf("%d",a[j]);
    return 0;
}

