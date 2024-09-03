#ifndef UTIL_H
#define UTIL_H
#include "floor.h"
#include "elevator.h"
#include "building.h"

typedef char* String;

typedef struct
{
    int id;
    struct node* prev;
    struct node* next;
} node;

route trimString(String str, building b);

#endif