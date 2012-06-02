#include <stdio.h>
#include <stdlib.h>

int main()
{
    int score,grade;
    printf("please input your score: ");
    scanf("%d",&score);
    if(score>=80)
        grade=1;
    else if(score>=60)
        grade=2;
    else 
        grade=3;
    switch(grade)
    {
        case 1: printf("You are a perfect student!\n");
	        break;
        case 2: printf("Do not play so more!!\n");
                break;
        case 3: printf("You must study harder!!!\n");
                break;
    }

    return 0;
}

