#include "start.h"
#include "driver/elevio.h"
#include "queue.h"
#include "lamp.h"
#include "interrupt.h"

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
    elevio_doorOpenLamp(0);

    //Starting the main program:

    Queue mainQueue;
    initQueue(&mainQueue);

    int nextFloor = get_first_Queue(&mainQueue);
    int floor = elevio_floorSensor();
    int openTime = -1;
    int startTime = -1;
    int doorOpen = 0;
    int prevFloor = 0;
    int prevDirn = 0;
    

    while(1){
        floor = elevio_floorSensor();
        if (floor != -1){
            prevFloor = floor;
        }
        set_floor_indicator(floor);

        //Håndterer neste i køen
        if(mainQueue.size > 0){
            if(floor == nextFloor){
                if(startTime == -1){
                    startTime = time(NULL);
                    elevio_motorDirection(DIRN_STOP);
                    elevio_doorOpenLamp(1);
                    doorOpen = 1;
                }
                else if(openTime >= DOOR_OPEN_TIME){
                    elevio_doorOpenLamp(0);
                    doorOpen = 0;
                    pop_front_Queue(&mainQueue);
                    if(mainQueue.size > 0){
                        nextFloor = get_first_Queue(&mainQueue);
                        if(floor < nextFloor){
                            elevio_motorDirection(DIRN_UP);
                            prevDirn = 1;
                        }
                        else if(floor > nextFloor){
                            elevio_motorDirection(DIRN_DOWN);
                            prevDirn = -1;
                        }
                    }
                    openTime = -1;
                    startTime = -1;
                }
                else{
                    openTime = time(NULL) - startTime;
                }
            }
        }

        
        //Hindrer å legge til mer enn de fire etasjene. 
        if(mainQueue.size < MAX_QUEUE_SIZE){
            for(int f = 0; f < N_FLOORS; f++){
                for(int b = 0; b < N_BUTTONS; b++){
                    int btnPressed = elevio_callButton(f, b);
                    if (btnPressed){
                        elevio_buttonLamp(f,b,1);
                        //sjekker om den kan legge til etasjen
                        if(appendQueue(f,&mainQueue)){
                            //setter motorretning hvis den var tom før denne ble appended
                            if(mainQueue.size == 1){
                                nextFloor = get_first_Queue(&mainQueue);
                                if(floor != -1){
                                    if(floor < nextFloor){
                                        elevio_motorDirection(DIRN_UP);
                                    }
                                    else if(floor > nextFloor){
                                        elevio_motorDirection(DIRN_DOWN);
                                    }
                                }
                                else{
                                    if(prevFloor < nextFloor){
                                        elevio_motorDirection(DIRN_UP);
                                    }
                                    else if(prevFloor > nextFloor){
                                        elevio_motorDirection(DIRN_DOWN);
                                    }
                                    else{
                                        if(prevDirn == -1){
                                            elevio_motorDirection(DIRN_UP);
                                        }
                                        else{
                                            elevio_motorDirection(DIRN_DOWN);
                                        }
                                    }
                                }
                                
                            }
                        }
                    }
                }
            }
        }
        
        stopInterrupt(floor, &mainQueue);
        
        obstructionInterrupt(doorOpen);
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
};