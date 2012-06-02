#include <stdio.h>
#include <stdlib.h>

int main()
{
    enum {sun,mon,tue,wed,thu,fri,sat} date;
    date=mon;
    printf("%d",date);
    return 0;
}

