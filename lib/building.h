#ifndef BUILDING_H
#define BUILDING_H
#include <stdio.h>
#include <stdlib.h>
#include "elevator.h"
#include "floor.h"
#include "util.h"

typedef struct
{
    String name;
    int noFloors;
    floor* floors;
    floor ground;
    elevator mainElevator;
} building;

#endif