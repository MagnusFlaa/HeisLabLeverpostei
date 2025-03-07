#include "queue.h"
#include "driver/elevio.h"

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
    int floor = q->bestillinger[0];
    elevio_buttonLamp(floor,0,0);
    elevio_buttonLamp(floor,1,0);
    elevio_buttonLamp(floor,2,0);
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

void clear_queue(Queue *q){
    for(int i = 0 ; i < q->size; i++){
        q->bestillinger[i] = 0 ;
    }
    q->size = 0;
}