#include <stdio.h>
#include <stdlib.h>

int main()
{
    int m=1,j=6;
    while(m)
    {
        j=j+m;
        m=m-1;
    }
    printf("%d",j);
    return 0;
}
