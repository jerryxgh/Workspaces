/* @(#)stackar.c
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

Stack CreateStack( int MaxElements ) {
    if (MaxElements < MinStackSize)
        Error("Stack size is too small.");

    Stack S = malloc(sizeof(struct StackRecord));
    if (S == NULL)
        FatalError("Out of sapce!");
    
    S->Array = malloc(MaxElements * sizeof(ElementType)); 
    if (S->Array == NULL)
        FatalError("Out of space!");
    S->Capacity = MaxElements;
    MakeEmpty(S);
    return S;
}

void MakeEmpty(Stack S) {
    S->TopOfStack = EmptyTOS;
}

void Push( ElementType X, Stack S ) {
    if (IsFull(S))
        FatalError("Stack is full!");
    else
        S->Array[++S->TopOfStack] = X;
}

ElementType Top( Stack S ) {
    if (IsEmpty(S)) {
        Error("Stack is empty!");
        return 0;
    } else
        return S->Array[S->TopOfStack];
}

void Pop( Stack S ) {
    if (IsEmpty(S))
        Error("Stack is empty!");
    else
        --S->TopOfStack;
}

ElementType TopAndPop( Stack S ) {
    if (IsEmpty(S)) {
        Error("Stack is empty!");
        return 0;
    } else
        return(S->Array[S->TopOfStack--]);
}
