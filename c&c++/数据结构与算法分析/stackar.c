* @(#)stackar.c
 */

#include "stackar.h"
#include "fatal.h"
#include <stdlib.h>

#define EmptyTOS ( -1 )
#define MinStackSize ( 5 )

struct StackRecord {
    int Capacity;
    int TopOfStack;
    ElementType *Array;
};

int IsEmpty(Stack S) {
    return S->TopOfStack == EmptyTOS;
}

int IsFull(Stack S) {
    return S->Capacity == S->TopOfStack + 1;
}
