#include "interrupt.h"
#include <stdio.h>
#include <time.h>


void stopInterrupt(int floor, Queue *q){
    if (elevio_stopButton()){
        elevio_motorDirection(DIRN_STOP);
        elevio_stopLamp(1);
        printf("Stop button pressed. Elevator stopped./n");
        clear_queue(q);
        if(floor != -1){
            elevio_doorOpenLamp(1);
        }

        while(elevio_stopButton()){
            nanosleep(&(struct timespec){1, 0}, NULL); //vente 0.1 sek til neste gang den sjekker
        }

        nanosleep(&(struct timespec){3, 0}, NULL); //vente 3 sek til starte igjen

        printf("Stop button released. Elevator will resume operations./n");
        obstructionInterrupt(1);
        elevio_doorOpenLamp(0);
        elevio_stopLamp(0);
        //muligens legge til DIRN_STOP her
            
    }
    

}


void obstructionInterrupt(int doorOpen){

    bool condition = elevio_obstruction() && doorOpen;
  
    if(condition){
        elevio_motorDirection(DIRN_STOP);
        elevio_stopLamp(1);
        printf("Obstruction detected. Elevator stopped./n");

        while(condition){
            condition = elevio_obstruction() && doorOpen;
            nanosleep(&(struct timespec){1, 0}, NULL); //vente 0.1 sek til neste gang den sjekker
        }
        
        nanosleep(&(struct timespec){3, 0}, NULL); //vente 3 sek til starte igjen

        printf("Obstruction was removed. Elevator will resume operations./n");

        elevio_stopLamp(0);

        
    }
}