/* @(#)stackli.c
 */
=
#include "stackli.h"
#include "fatal.h"
#include <stdlib.h>

struct Node {
    ElementType Element;
    PtrToNode Next;
};

int IsEmpty(Stack S) {
    return S.Next == NULL;
}

Stack CreateStack() {
    Stack S;
    S = malloc(sizeof(struct Node));
    if (S == NULL)
        FatalError("Out of space!");
    S->Next = NULL;
    MakeEmpty(S);
    return S;
}

void MakeEmpty(Stack S) {
    if (S == NULL)
        Error("Must use CreateStack first!");
    else
        while (!IsEmpty(S))
            Pop(S);
}

void DisposeStack(S) {
    MakeEmpty(S);
    free(S);
}

void Push(ElementType X, Stack S) {
    PtrToNode N;
    N = malloc(sizeof(struct Node));
    if (N == NULL)
        FatalError("Out of space!");
    else {
        N->Element = X;
        N->Next = S->Next;
        S->Next = N;
    }
}

ElementType Top(Stack S) {
    if (!IsEmpty(S))
        return S->Next->Element;
    else
        Error("Empty stack!");
    return 0;
}

void Pop(Stack S) {
    PtrToNode FirstCell;;
    if (!IsEmpty(S)) {
        FirstCell = S->Next;
        S->Next = S->Next->Next;
        free(FirstCell);
    }
}
