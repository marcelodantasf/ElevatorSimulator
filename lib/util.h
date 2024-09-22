#ifndef UTIL_H
#define UTIL_H
#include "floor.h"
#include "elevator.h"
#include "building.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define KEY "_"

typedef char* String;

typedef struct
{
    int number;
    struct floor* prev;
    struct floor* next;
} floor;

char *overwriteComma(char *str);

void* trimString(String str, building* b, int noElevators);

#endif