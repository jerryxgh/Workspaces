#include <stdio.h>
#include <stdlib.h>

#define PI 3.1415926
#define area(r) PI*r*r

int main()
{
    float r,a; 
    scanf("%f",&r);
    a=area(r);
    printf("The area is %f\n",a);
    return 0;
}

