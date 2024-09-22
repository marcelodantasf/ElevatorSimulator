#ifndef ELEVATOR_H
#define ELEVATOR_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "building.h"
#include "util.h"
#include "floor.h"

typedef enum 
{
    DOWN,
    UP,
    STOPPED
} way;

typedef struct
{
    floor* origin;
    floor* destiny;
    way dir;
    int time;
    struct call* next;
} call;

typedef struct
{
   floor* current;
   int inactivity;
   way direction;
   call* calls;
} elevator;

void standByMode(elevator* e, floor* ground);

call addToRoute(floor* f, elevator* e, way d);

elevator* algorithm(floor* f, building b);

void move(elevator* e);

#endif
