#pragma once

#include <stdbool.h>

#define MAX_QUEUE_SIZE 4

typedef struct{
    int bestillinger[MAX_QUEUE_SIZE];
    int size;
    int front;
    int rear;
}Queue;

void initQueue(Queue *q);

bool appendQueue(int floor, Queue *q);

bool pop_front_Queue(Queue *q);