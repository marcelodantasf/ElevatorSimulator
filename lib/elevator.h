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
    UP
} way;

//typedef enum Way way;

/*typedef struct
{
    int noButtons;
    floor* selected;
} internalButton;

typedef struct
{
   way direction;
   bool isPressed;
} externalButton; */


typedef struct
{
   floor* current;
   //internalButton ib;
   //externalButton eb;
   floor* selected;
} elevator;

typedef struct
{
    floor* destiny;
    way dir;
} route;

elevator standByMode(elevator* e, floor* ground);

//void toggleButton(elevator* e);

route addToRoute(floor* f, elevator* e, way d);

elevator whichCloser(floor* f, building b);

void move(elevator* e);

#endif
