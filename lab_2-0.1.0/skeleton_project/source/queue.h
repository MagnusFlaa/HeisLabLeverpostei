#pragma once

#include <stdbool.h>

#define MAX_QUEUE_SIZE 4

typedef struct{
    int bestillinger[MAX_QUEUE_SIZE];
    int size;
}Queue;

void initQueue(Queue *q);

bool appendQueue(int floor, Queue *q);

void pop_front_Queue(Queue *q);

int get_first_Queue(Queue *q);

bool is_in_Queue(int floor, Queue *q);