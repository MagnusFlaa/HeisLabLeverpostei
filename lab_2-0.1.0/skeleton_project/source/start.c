#include "start.h"
#include "driver/elevio.h"
#include "queue.h"
#include "lamp.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


void goTo0(){
    elevio_motorDirection(DIRN_DOWN);
    while(1){
        int floor = elevio_floorSensor();
        if (floor == 0){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

}

void start(){
    //getting into start position
    goTo0();

    // For å signalisere at den har funnet stedet sitt.
    elevio_stopLamp(1);  // Tenn stopp-lampa
    nanosleep(&(struct timespec){1, 0}, NULL);  // Vent 1 sekund
    elevio_stopLamp(0); //Slukk lampa

    //Starting the main program:

    Queue mainQueue;
    initQueue(&mainQueue);

    int nextFloor = get_first_Queue(&mainQueue);
    int floor = elevio_floorSensor();

    while(1){

        //Håndterer neste i køen
        if(mainQueue.size > 0){
            floor = elevio_floorSensor();
            if(floor == nextFloor){
                elevio_motorDirection(DIRN_STOP);
                nanosleep(&(struct timespec){2, 0}, NULL);  // Vent 2 sekunder
                pop_front_Queue(&mainQueue);
                if(mainQueue.size > 0){
                    nextFloor = get_first_Queue(&mainQueue);
                    if(floor < nextFloor){
                        elevio_motorDirection(DIRN_UP);
                    }
                    else if(floor > nextFloor){
                        elevio_motorDirection(DIRN_DOWN);
                    }
                }
                
            }
        }

        
        //Hindrer å legge til mer enn de fire etasjene. 
        if(mainQueue.size < MAX_QUEUE_SIZE){
            for(int f = 0; f < N_FLOORS; f++){
                for(int b = 0; b < N_BUTTONS; b++){
                    int btnPressed = elevio_callButton(f, b);
                    if (btnPressed){
                        //sjekker om den kan legge til etasjen
                        if(appendQueue(f,&mainQueue)){
                            //setter motorretning hvis den var tom før denne ble appended
                            if(mainQueue.size == 1){
                                nextFloor = get_first_Queue(&mainQueue);
                                if(floor < nextFloor){
                                    elevio_motorDirection(DIRN_UP);
                                }
                                else if(floor > nextFloor){
                                    elevio_motorDirection(DIRN_DOWN);
                                }
                            }
                        }
                    }
                }
            }
        }
        if(elevio_obstruction()){
            elevio_stopLamp(1);
        } else {
            elevio_stopLamp(0);
        }
        
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
        
        elevio_floorIndicator(floor);
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
};