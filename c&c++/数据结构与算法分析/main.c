/* @(#)main.c
 */

#include <stdio.h>

#define IN 0
#define OUT 1

int main(int argc, char *argv[]) {
    int c, cc, cw, cl, state;

    state = OUT;
    cc = cw = cl = 0;
    while ((c = getchar()) != EOF) {
        ++cc;
        if (c == '\n')
            ++cl;
        if (c == ' ' || c == '\t' || c == '\n')
            state = OUT;
        else if (state == OUT) {
            state = IN;
            ++cw;
        }
    }
    
    printf ("character number: %d\nline number: %d\nword number %d\n", cc, cl, cw);

    return 0;
}
