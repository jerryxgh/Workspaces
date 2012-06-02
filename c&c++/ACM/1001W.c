/* @(#)main.c
 */
#include <stdio.h>
#include <stdlib.h>

#define Error( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )

struct Node;
struct Queue;
typedef struct Node *PtrToNode;
typedef struct Queue *PtrToQueue;
struct Node {
    int element;
    PtrToNode next;
};

struct Queue {
    PtrToNode front;
    PtrToNode rear;
};
    
PtrToQueue createQueue();
void enqueue(int x, PtrToQueue q);
int dequeue(PtrToQueue q);
int isQueueEmpty(PtrToQueue q);

int sum(int i);


int main() {
    PtrToQueue q = createQueue();
    int i;
    while (scanf("%d", &i) != EOF)
        enqueue(i, q);
    while (!isQueueEmpty(q)) {
        printf ("%d\n", sum(dequeue(q)));
        if (!isQueueEmpty(q))
            putchar('\n');
    }
    
    free(q);
    return 0;
}

int sum(int i) {
    return i * (i + 1) / 2;
}

PtrToQueue createQueue() {
    PtrToQueue q;
    q = malloc(sizeof(struct Queue));
    if (q == NULL)
        Error("Out of space!");
    q->front = NULL;
    q->rear = NULL;
    return q; 
}

void enqueue(int x, PtrToQueue q) {
    PtrToNode tempCell;
    tempCell = malloc(sizeof(struct Node));
    if (tempCell == NULL)
        Error("Out of space!");
    tempCell->element = x;
    tempCell->next = NULL;
    if (q->rear == NULL)
        q->front = q->rear = tempCell;
    else {
        q->rear->next = tempCell;
        q->rear = tempCell;
    }
}

int dequeue(PtrToQueue q) {
    int temp;
    PtrToNode tempCell = q->front;
    if (tempCell == NULL)
        Error("Empty queue!");
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    temp = tempCell->element;
    free(tempCell);
    return temp;
}

int isQueueEmpty(PtrToQueue q) {
    return q->front == NULL;
}

void emptyQueue(PtrToQueue q) {
    while (!isQueueEmpty(q))
        dequeue(q);
}
