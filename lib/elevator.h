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

typedef enum Way way;

typedef struct
{
    int noButtons;
    floor* selected;
} internalButton;

typedef struct
{
   way direction;
   bool isPressed;
} externalButton;


typedef struct
{
   floor* current;
   internalButton ib;
   externalButton eb;
} elevator;

elevator standByMode(elevator* e, floor* ground);

void toggleButton(elevator* e);

#endif
