#ifndef __MOVIES_H__
#define __MOVIES_H__

#include <stdlib.h>
#include <math.h>

typedef struct node
{
	int v;
	char *film;
	char *actor;
	struct node *next;
} TNode, *ATNode;

typedef struct
{
	int nr_actori;
	int nr_filme;
	char **actori;
	char **filme;
	int ActorX;
	int ActorY;
	ATNode *adl;
}TGraphL;

typedef struct punte
{
    char *Actor1;
    char *Actor2;
    struct punte *next;
}Punti;

typedef struct MinHeapNode
{
    int  v;
    int d;
} MinHeapNode;


typedef struct MinHeap
{
    int size;
    int capacity;
    int *pos;
    MinHeapNode **elem;
} MinHeap;


MinHeapNode* newNode(int v, int d)
{
    MinHeapNode* n =(MinHeapNode*) malloc(sizeof(MinHeapNode));
    n->v = v;
    n->d = d;
    return n;
}

MinHeap* newQueue(int capacity)
{
    MinHeap* h =(MinHeap*) malloc(sizeof(MinHeap));
    h->pos = (int *)malloc(capacity * sizeof(int));
    h->size = 0;
    h->capacity = capacity;
    h->elem =(MinHeapNode**) malloc(capacity * sizeof(MinHeapNode*));
    return h;
}

void swap(MinHeapNode** a, MinHeapNode** b)
{
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void SiftDown(MinHeap* h, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < h->size && h->elem[left]->d < h->elem[smallest]->d )
      smallest = left;

    if (right < h->size && h->elem[right]->d < h->elem[smallest]->d )
      smallest = right;

    if (smallest != idx)
    {
        MinHeapNode *a = h->elem[smallest];
        MinHeapNode *b = h->elem[idx];
        h->pos[a->v] = idx;
        h->pos[b->v] = smallest;
        swap(&h->elem[smallest], &h->elem[idx]);
        SiftDown(h, smallest);
    }
}

int isEmpty(MinHeap* h)
{
    return h->size == 0;
}

MinHeapNode* removeMin(MinHeap* h)
{
    if (isEmpty(h))
        return NULL;
    MinHeapNode* min = h->elem[0];
    MinHeapNode* last = h->elem[h->size - 1];
    h->elem[0] = last;
    h->pos[min->v] = h->size-1;
    h->pos[last->v] = 0;
    h->size--;
    SiftDown(h, 0);
    return min;
}

void SiftUp(MinHeap* h, int v, int d)
{
    int i = h->pos[v];
    h->elem[i]->d = d;
    while (i && h->elem[i]->d < h->elem[(i - 1) / 2]->d)
    {
        h->pos[h->elem[i]->v] = (i-1)/2;
        h->pos[h->elem[(i-1)/2]->v] = i;
        swap(&h->elem[i],  &h->elem[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int isInMinHeap(MinHeap *h, int v)
{
   if (h->pos[v] < h->size)
     return 1;
   return 0;
}

#endif