#include "queue.h"
#include "page.h"

Queue *queue;

Queue *createQueue() {
    queue = (Queue *)malloc(sizeof(Queue));
    queue->capacity = MP;
    queue->front = queue->size = 0;
    queue->rear = 0;
    queue->array = (Page *)malloc(queue->capacity * sizeof(Page));

    queue->inqueue = inqueue;
    queue->dequeue = dequeue;
    queue->peek = peek;

    return queue;
}

void inqueue(Page *page) {
    if (queue->size == queue->capacity) {
        return;
    }
    queue->array[queue->rear] = *page;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->size = queue->size + 1;
}

void dequeue() {
    if (queue->size == 0) {
        return;
    }
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
}

Page *peek() {
    if (queue->size == 0) {
        return NULL;
    }
    return &queue->array[queue->front];
}
