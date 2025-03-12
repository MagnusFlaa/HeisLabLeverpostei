#include "lamp.h"
#include "driver/elevio.h"



void set_floor_indicator(int floor){
    if(floor >= 0){
        elevio_floorIndicator(floor);
        
    }
}

void open_door(){
    elevio_doorOpenLamp(1);
}

void close_door(){
    elevio_doorOpenLamp(0);
}