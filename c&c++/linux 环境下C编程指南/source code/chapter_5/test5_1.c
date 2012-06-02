#include <stdio.h>
int getinput(void); 
void printmessage(int counter,int input);

int main(void)
{
	int counter;
	int input;

	for(counter=0;counter<=200;counter++)
	{
		input=getinput();
		if(input==-1) end(0);
		printmessage(counter,input);
	}

	return 0;
}

int getinput(void)
{
	int input;

	printf("Enter an integer,or use -1 to exit:");
	scanf("%d",&input);
	return input;
}

void printmessage(int counter,int input)
{
	static int lastnum=0;
	counter++;

	printf("For number %d ,you entered %d (%d more than last time)\n",
			counter,input,input-lastnum);
	lastnum=input;
}
