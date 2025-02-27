#include "interrupt.h"

void stopInterrupt(){
    if (elevio_stopButton()){
        elevio_motorDirection(DIRN_STOP);
        elevio_stopLamp(1);
        printf("Stop button pressed. Elevator stopped.");
    }

}

void obstructionInterrupt(){
    if(elevio_obstruction()){
        elevio_motorDirection(DIRN_STOP);
        elevio_stopLamp(1);
        printf("Obstruction detected. Elevator stopped.");
    }
}
