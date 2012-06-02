/* @(#)main.c
 */
#include <stdio.h>
int sum(int n);

int main() {
    int i;
    while(scanf("%d", &i) != EOF) {
        printf("%d\n\n", sum(i));
    }
    return 0;
}

int sum(int n) {
    int result = 0;
    if (n > 0)
        result = n * (n + 1) / 2;
    return result;
}
