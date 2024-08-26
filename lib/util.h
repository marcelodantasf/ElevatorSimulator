#ifndef UTIL_H
#define UTIL_H

typedef char* String;

typedef struct
{
    int id;
    struct node* prev;
    struct node* next;
} node;

#endif