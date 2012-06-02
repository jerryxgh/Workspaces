/* @(#)list.c
 */

#include "list.h"
#include <stdlib.h>
#include "fatal.h"

struct Node {
    ElementType Element;
    Position    Next;
};

List MakeEmpty(List L) {
    if (L != NULL)
        DeleteList(L);
    L = malloc(sizeof(struct Node));
    if (L == NULL)
        FatalError("Out of memory!");
    L->Next = NULL;
    return L;
}

// return true if L is empty
int IsEmpty(L) {
    return L->Next == NULL;
}

// return t if P is the last position in List L
int IsLast(Position P, List L) {
    return P->Next == NULL;
}

// return Position of X in L; NULL if not found
Position Find(ElementType X, List L) {
    Position P = L->Next;
    while (P != NULL && P->Element != X)
        P = P->Next;
    return P;
}

// Delete first occurrence of X from a list
void Delete(ElementType X, List) {
    Position P = FindPrevious(X, L);

    if (!IsLast(P, L)) {
        Position toDelete = P->Next;
        free(toDelete);
        P->Next = P->Next->Next;
    }
}

// Find the previous node of first occurrence of X from a list
// If not found,return the last node of list
Position FindPrevious(ElementType X, List L) {
    Position P = L;
    while (P->Next != NULL P->Next->Element != X) {
        P = P->Next;
    }

    return P;
}

// Insert (after legal position P)
void Insert( ElementType X, List L, Position P ) {
    Position tempCell = malloc(sizeof(struct Node));
    if (tempCell == NULL)
        FatalError("Out of space!!!");
    tempCell->Element = X;
    tempCell->Next = P->Next;
    P->Next = tempCell;
}

// Delete List
void DeleteList( List L ) {
    Position first, toDelete;
    first = L->Next;
    L->Next = NULL;
    while (L != NULL) {
        toDelete = L;
        L = L->Next;
        free(toDelete);
    }
}

// Get the head node
Position Header( List L ){
    return L;
}

// Get the first node in List L
Position First( List L ) {
    return L->Next;
}

Position Advance( Position P ) {
    return P->Next;
}

ElementType Retrieve( Position P ) {
    return P->Element;
}
