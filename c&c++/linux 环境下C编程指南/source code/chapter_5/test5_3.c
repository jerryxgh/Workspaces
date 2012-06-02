#include <stdio.h>

int main(void)
{
    int counter; 
    for(counter=0;counter<30;counter++)
    {
        if(counter%2==0)
        {
            printf("Counter:%d\n",counter);
        }
    }
}
