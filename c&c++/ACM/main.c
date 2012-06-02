/* @(#)main.c
 */
#include <stdio.h>

void bigSum(int* A, int* B, int* C);
void getBigNumber(int * N);
void swap(int *N, int x, int y);
void printBigNumber(int *N);

int main() {
    int num, i;
    int A[1000], B[1000], C[1001];
    scanf("%d", &num);
    for (i = 0; i < num; ++i) {
        getBigNumber(A);
        getBigNumber(B);
        bigSum(A, B, C);
        printf("Case %d:\n", i + 1);
        printBigNumber(A);
        printf (" + ");
        printBigNumber(B);
        printf (" = ");
        printBigNumber(C);
        printf ("\n");

        if (i < num - 1)
            putchar('\n');
    }
    return 0;
}

void bigSum(int* A, int* B, int* C) {
    int i, j, temp, carry;
    for (i = 0, carry = 0; A[i] != 0 && B[i] != 0; ++i) {
        temp = A[i] + B[i] + carry;
        carry = (temp - 2 * '0') / 10;
        C[i] = temp - '0' - carry * 10;
    }

    if (A[i] == 0) {
        if (B[i] == 0) {
            C[i] = carry;
            if (carry != 0) {
                C[i] += '0';
                C[i + 1] = 0;
            } 
        } else {
            for (j = 0; B[i + j] != 0; ++j) {
                temp = B[i + j] + carry;
                carry = (temp - '0') / 10;
                C[i + j] = temp - carry * 10;
            }
            C[i + j] = carry;
            if (carry != 0) {
                C[i + j] += '0';
                C[i + j + 1] = 0;
            } 
        }
    } else {
        if (A[i] == 0) {
            C[i] = carry;
            if (carry != 0) {
                C[i] += '0';
                C[i + 1] = 0;
            } 
        } else {
            for (j = 0; A[i + j] != 0; ++j) {
                temp = A[i + j] + carry;
                carry = (temp - '0') / 10;
                C[i + j] = temp - carry * 10;
            }
            C[i + j] = carry;
            if (carry != 0) {
                C[i + j] += '0';
                C[i + j + 1] = 0;
            } 
        }
    }
}

void getBigNumber(int * N) {
    int c, i, j, k;
    while ((c = getchar()) == '\n' || c == ' ')
        ;
    *N = c;

    for (i = 1; (c = getchar()) != '\n' && c != ' '; ++i) {
        *(N + i) = c;
    }
    *(N + i) = 0;

    j = i / 2;
    for (k = 0; k < j; ++k)
        swap(N, k, i - 1 - k);
}

void swap(int *N, int x, int y) {
    if (x == y) return;
    int temp;
    temp = *(N + x);
    *(N + x) = *(N + y);
    *(N + y) = temp;
}

void printBigNumber(int *N) {
    int i, j;
    for (i = 0; *(N + i) != 0; ++i)
        ;
    for (j = i; j > 0;)
        putchar(*(N + --j));
}
