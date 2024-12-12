#ifndef QUEUE_H
#define QUEUE_H

typedef struct Page Page;

typedef struct {
    int capacity, front, rear, size;
    Page *array;

    void (*inqueue)(Page *page);
    void (*dequeue)();
    Page *(*peek)();
} Queue;

extern Queue *queue;

Queue *createQueue();
void   inqueue(Page *page);
void   dequeue();
Page  *peek();

#endif