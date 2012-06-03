/* @(#)main.c
*/

#include <stdio.h>

void printMostRepeat(char * input);

int main() {
    char input[100001], temp;
    int i;
    
    while ((temp = getchar()) != '#') {
        // get input
        if (temp == '\n') {
            input[0] = '\0';
            break;
        } else input[0] = temp;

        for (i = 1; (temp = getchar()) != '\n'; ++i) {
            input[i] = temp;
        }
        input[i] = '\0';

        printMostRepeat(input);
    }
}

void printMostRepeat(char * input) {
    
}

