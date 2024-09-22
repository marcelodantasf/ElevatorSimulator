#ifndef BUILDING_H
#define BUILDING_H
#include <stdio.h>
#include <stdlib.h>
#include "elevator.h"
#include "floor.h"
#include "util.h"

typedef struct
{
    int noFloors;
    int noElevators;
    floor* floors;
    floor* ground;
    elevator* elevators;
    call* calls;
} building;

void initialize(building* b, int noElevators, int noFloors);

void* trimString(String str, building* b, int noElevators);

#endif