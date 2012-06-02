/* @(#)main.c
 */
#include <stdio.h>

int main(int argc, char *argv[]) {
    int i, j;
    while (scanf("%d%d", &i, &j) != EOF)
        printf ("%d\n",i + j);
    printf ("%d\n", scanf("%d%d", &i, &j));
    return 0;
}
