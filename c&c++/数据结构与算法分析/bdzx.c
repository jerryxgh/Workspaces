/* @(#)main.c
*/
#include <stdio.h>
#include <stdlib.h>

#define NUM 26 /* 字母的个数 */

void initLoaction(int * location);

int main(int argc, char *argv[]) {
    char input[100001];
    int location[NUM], previous[100001];
    initLoaction(location);
    scanf("%s", input);
    while (input[0] != '#') {
        initLoaction(location);
        int length, i;
        for (i = 0; input[i] != '\0'; ++i) {
            previous[i] = location[input[i] - 'a'];
            location[input[i] - 'a'] = i;
        }
        length = i;

        for (i = 0; i < length; ++i) {
            printf ("%d\n",previous[i]);
        }

        scanf("%s", input);
    }

    return 0;
}

void initLoaction(int * location) {
    for (int i = 0; i < NUM; ++i) { /* Initialize location */
        location[i] = -1;
    }
}
