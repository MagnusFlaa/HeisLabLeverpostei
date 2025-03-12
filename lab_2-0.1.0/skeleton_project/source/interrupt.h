#pragma once

#include <stdbool.h>
#include "driver/elevio.h"
#include "queue.h"
#include "lamp.h"


void stopInterrupt(int floor, Queue *q);

void obstructionInterrupt(int doorOpen);