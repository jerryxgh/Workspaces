/* @(#)main.c
 */

#include <stdio.h>

void printArray(int var[], int length);
void qsort(int var[], int begin, int end);
void swap(int var[], int a, int b);
    
int main(int argc, char *argv[]) {
    int var[] = {1, 3, 4 , 5, 9, 2, 100, 23, 433, 54};
    printArray(var, 10);
    qsort(var, 0, 9);
    printArray(var, 10);
    
    return 0;
}

void printArray(int var[], int length) {
    printf ("数组的内容如下：\n");
    for (int i = 0; i < length; ++i) {
        printf ("%d ",var[i]);
    }
    printf ("\n");
}

void qsort(int var[], int begin, int end) {
    int middle = (begin + end) / 2;

    if (begin >= end) // 递归终止条件，完成排序
        return;

    swap(var, begin, middle);

    // 没一次循环结束后，第 k 个数之前的所有数字，满足 var[k-1] <= var[0]
    int k, i;
    for (i = begin + 1, k = begin + 1; i <= end; ++i) {
        if (var[i] < var[begin])
            swap(var, k++, i);
    }
    swap(var, begin, k - 1);
    qsort(var, begin, k - 2);
    qsort(var, k, end);
}

void swap(int var[], int a, int b) {
    int temp;
    temp = var[a];
    var[a] = var[b];
    var[b] = temp;
}
