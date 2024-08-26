#include "elevator.h"

elevator standByMode(elevator* e, floor* ground)
{
   e->current = ground;
   printf("\nELEVATOR ENTERED STAND-BY MODE!\nheading to ground floor\n\n");
}