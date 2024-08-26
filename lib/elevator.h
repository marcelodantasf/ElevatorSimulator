#ifndef ELEVATOR_H
#define ELEVATOR_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"
#include "floor.h"

enum Way
{
    DOWN,
    UP
};

typedef struct
{
    int noFloors;
    floor* selected;
} internalButton;

typedef struct
{
   Way direction;
   bool isPressed;
} externalButton;


typedef struct
{
   floor* current;
   //floor* selected;
   floor* prev;
   floor* next;
   internalButton ib;
   externalButton eb;
} elevator;

elevator standByMode(elevator* e, floor* ground);

void toggleButton(elevator* e);

#endif
