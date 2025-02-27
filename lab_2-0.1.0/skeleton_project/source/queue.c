#include "queue.h"

void initQueue(Queue *q){
    q->size = 0;
}
bool appendQueue(int floor, Queue *q){
    if(is_in_Queue(floor, q)){
        return false;
    }
    else{
        q->bestillinger[q->size] = floor;
        q->size += 1;
        return true;
    }
}

void pop_front_Queue(Queue *q){
    for(int i = 0; i < q->size-1; i++){
        q->bestillinger[i] = q->bestillinger[i+1];
    }
    q->size -= 1;
    q->bestillinger[q->size] = 0;
}

int get_first_Queue(Queue *q){
    if(q->size <= 0){
        return -2;
    }
    return q->bestillinger[0];
}

bool is_in_Queue(int floor, Queue *q){
    for(int i = 0; i < q->size; i++){
        if(floor == q->bestillinger[i]){
            return true;
        }
    }
    return false;
}