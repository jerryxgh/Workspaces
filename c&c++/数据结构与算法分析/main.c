/* @(#)main.c
*/
#include <stdio.h>
#include <stdlib.h>

#define NUM 26 /* 字母的个数 */

int main(int argc, char *argv[]) {
    char input[100001];
    int temp[NUM], lastChar[100001];
    for (int i = 0; i < NUM; ++i) { /* Initialize temp */
        temp[i] = -1;
    }
    scanf("%s", input);
    while (input[0] != '#') {
        int length, i;
        for (i = 0; input[i] != '\0'; ++i) {
            lastChar[i] = temp[input[i] - 'a'];
            temp[input[i] - 'a'] = i;
        }
        length = i;

        for (i = 0; i < length; ++i) {
            printf ("%d\n",lastChar[i]);
        }

        scanf("%s", input);
    }

    return 0;
}
