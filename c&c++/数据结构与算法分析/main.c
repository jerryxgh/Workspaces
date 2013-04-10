/* @(#)main.c
 */

#include <stdio.h>
#include <unistd.h>
#include "stackar.h"

void printArray(int array[], int length);
void swap(int array[], int a, int b);
void perm(int list[], int k, int m);

int main(int argc, char *argv[]) {
    int A[] = {1, 2, 3};
    int length = 10;
    perm(A, 0, 2);
}

// 求 list[k:m]的全排列
void perm(int list[], int k, int m) {
    if (k == m) {
        printArray(list, m + 1);
    }
    else {
        for (int i = k; i <= m; ++i) {
            swap(list, k, i);
            perm(list, k + 1, m);
            swap(list, k, i);
        }
    }
}

void printArray(int array[], int length) {
    for (int i = 0; i < length; ++i) {
        printf("%d", array[i]);
        if (i == (length - 1))
            printf("\n");
        else printf(" ");
    }
}

void swap(int array[], int a, int b) {
    int tmp = array[a];
    array[a] = array[b];
    array[b] = tmp;
}
