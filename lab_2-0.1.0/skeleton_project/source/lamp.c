#include "lamp.h"
#include "driver/elevio.h"

void set_floor_indicator(int floor){
    if(floor >= 0){
        elevio_floorIndicator(floor);
    }
}