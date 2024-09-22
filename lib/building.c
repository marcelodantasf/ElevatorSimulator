#include "building.h"

void initialize(building* b, int noElevators, int noFloors)
{
    b->calls = NULL;
    b->noElevators = noElevators;
    b->noFloors = noFloors;

    b->floors = malloc(noFloors * sizeof(floor));
    b->elevators = malloc(noElevators * sizeof(elevator));

    int i = 0;
    while(i < noFloors) {
        b->floors[i].number = i;

        if(i<1)
            b->floors[i].prev = NULL;
        else
            b->floors[i].prev = &b->floors[i-1];
    
        if(i >= noFloors)
            b->floors[i].next = NULL;
        else
            b->floors[i].next = &b->floors[i+1];

        i++;
    }

    b->ground = &b->floors[0];

    i = 0;
    while(i < noElevators) {
        b->elevators[i].inactivity = 0;
        b->elevators[i].calls = NULL;
        b->elevators[i].direction = STOPPED;
        b->elevators[i].current = &b->floors[0];
        i++;
    }
}

void* trimString(String str, building* b, int noElevators) {
    str = overwriteComma(str);

    if(str[0] == "A") {
        char *newString = strtok(str, KEY);
        newString = strtok(NULL, KEY);

        int no_floors = atoi(newString);

        initialize(b, noElevators, no_floors);
        return;
    }

    if(str[0] == "E") {
        char *newString = strtok(str, KEY);

        int noElevator = atoi(newString);
        noElevator -= 1;

        if (noElevator < 0 || noElevator+1 > b->noElevators) {
            printf("numero invalido\n");
            return;
        }

        newString = strtok(NULL, KEY);
        int beginningFloor = atoi(newString);
        
        b->elevators[noElevator].current = &b->floors[beginningFloor];

        newString = strtok(NULL, KEY);
        way direction = (strcmp(newString, "S") == 0) ? UP : DOWN;

        //b->elevators[noElevator].calls = newCall;

        newString = strtok(NULL, KEY);

        int floors[10]; // buffer para os andares
        int count = 0;

        while (newString != NULL) {

            if (newString[0] == 'T')
                floors[count] = 0;
            else
                floors[count] = atoi(newString);

            newString = strtok(NULL, KEY);
            count++;
        }

        call* newCalls = malloc(count * sizeof(call));
        newCalls[0].time = 0;
        newCalls[0].origin = &b->elevators[noElevator].current;
        newCalls[0].destiny = &b->floors[floors[0]];
        newCalls[0].dir = direction;
        newCalls[0].next = &newCalls[1];

        for(int i = 1; i < count; i++) {
            newCalls[i].destiny = &b->floors[floors[i]];
            newCalls[i].origin = &newCalls[i-1].destiny;
            newCalls[i].time = 0;
            newCalls[i].dir = direction;

            if(i++ == count)
                newCalls[i].next = NULL;
            else
                newCalls[i].next = &newCalls[i+1];
        }

        //nao ha necessidade de conferir se existem chamadas preexistentes
        //pois o case 'E' inicia o funcionamento dos elevadores.
        b->calls = newCalls;
    }

    //if(str[0] == "T"){}

    printf("Formato invalido, digite as instrucoes conforme as regras\n");
}